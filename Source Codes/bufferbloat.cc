#include "ns3/core-module.h"
#include "ns3/config-store-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/buildings-module.h"
#include "ns3/nr-module.h"
#include "ns3/antenna-module.h"
#include "ns3/traffic-control-module.h"


/*
 * Use, always, the namespace ns3. All the NR classes are inside such namespace.
 */
using namespace ns3;
using namespace std;
typedef std::vector<Ptr<QueueDisc> > QueueDiscVector;
/*
 * With this line, we will be able to see the logs of the file by enabling the
 * component "CttcNrDemo"
 */
//NS_LOG_COMPONENT_DEFINE ("CttcNrDemo");
NS_LOG_COMPONENT_DEFINE("BufferBloat");
void func(Ptr<NetDevice> abc,Ptr<NetDevice> abc2){
    
        std::cout<<abc->GetNode ()->GetObject<TrafficControlLayer> ()<<std::endl;
        std::cout<<abc2->GetNode ()->GetObject<TrafficControlLayer> ()<<std::endl;
        Ptr<TrafficControlLayer> tc = abc->GetNode ()->GetObject<TrafficControlLayer> ();
        //NS_LOG_DEBUG(tc->GetRootQueueDiscOnDevice(abc));

        //tc->SetRootQueueDiscOnDevice (d, m_queueDiscs[0]);
    //NS_LOG_DEBUG(abc->GetAttribute("ns3::RootQueueDiscList"));
    //std::map < uint8_t, Ptr<BandwidthPartUe> > b = abc2->GetCcMap();
    //for(auto i: b) NS_LOG_INFO(i.second->GetPhy()->PhyDataPacketReceived);

      //Simulator::Schedule(Seconds(0.1), &func, abc, abc2);

      
}

int
main (int argc, char *argv[])
{
  /*
   * Variables that represent the parameters we will accept as input by the
   * command line. Each of them is initialized with a default value, and
   * possibly overridden below when command-line arguments are parsed.
   */
  // Scenario parameters (that we will use inside this script):
  uint16_t gNbNum = 1;
  uint16_t ueNumPergNb = 1;
  bool logging = false;
  int64_t randomStream = 1;

  int scenario =1;
  // Traffic parameters (that we will use inside this script):
  uint32_t udpPacketSizeULL = 100;
  uint32_t udpPacketSizeBe = 1252;
  uint32_t lambdaULL = 10000;
  uint32_t lambdaBe = 1000;

  // Simulation parameters. Please don't use double to indicate seconds; use
  // ns-3 Time values which use integers to avoid portability issues.
  Time simTime = MilliSeconds (10000);
  Time udpAppStartTime = MilliSeconds (400);

  // NR parameters. We will take the input from the command line, and then we
  // will pass them inside the NR module.
  uint16_t numerologyBwp1 = 0;
  double centralFrequencyBand1 = 28e9;
  double bandwidthBand1 = 5e6;
  double totalTxPower = 23;

  // Where we will store the output files.
  std::string simTag = "default";
  std::string outputDir = "./";

  /*
   * From here, we instruct the ns3::CommandLine class of all the input parameters
   * that we may accept as input, as well as their description, and the storage
   * variable.
   */
  CommandLine cmd;

  cmd.AddValue ("gNbNum",
                "The number of gNbs in multiple-ue topology",
                gNbNum);
  cmd.AddValue ("ueNumPergNb",
                "The number of UE per gNb in multiple-ue topology",
                ueNumPergNb);
  cmd.AddValue ("logging",
                "Enable logging",
                logging);
  cmd.AddValue ("packetSizeUll",
                "packet size in bytes to be used by ultra low latency traffic",
                udpPacketSizeULL);
  cmd.AddValue ("packetSizeBe",
                "packet size in bytes to be used by best effort traffic",
                udpPacketSizeBe);
  cmd.AddValue ("lambdaUll",
                "Number of UDP packets in one second for ultra low latency traffic",
                lambdaULL);
  cmd.AddValue ("lambdaBe",
                "Number of UDP packets in one second for best effor traffic",
                lambdaBe);
  cmd.AddValue ("simTime",
                "Simulation time",
                simTime);
  cmd.AddValue ("numerologyBwp1",
                "The numerology to be used in bandwidth part 1",
                numerologyBwp1);
  cmd.AddValue ("centralFrequencyBand1",
                "The system frequency to be used in band 1",
                centralFrequencyBand1);
  cmd.AddValue ("bandwidthBand1",
                "The system bandwidth to be used in band 1",
                bandwidthBand1);
  cmd.AddValue ("totalTxPower",
                "total tx power that will be proportionally assigned to"
                " bands, CCs and bandwidth parts depending on each BWP bandwidth ",
                totalTxPower);
  cmd.AddValue ("simTag",
                "tag to be appended to output filenames to distinguish simulation campaigns",
                simTag);
  cmd.AddValue ("outputDir",
                "directory where to store simulation results",
                outputDir);
  cmd.AddValue ("scenario",
                "Scenario 1 for elephant and mice flow, scenario 2 for only mice flow",
                scenario);


  // Parse the command line
  cmd.Parse (argc, argv);

  /*
   * Check if the frequency is in the allowed range.
   * If you need to add other checks, here is the best position to put them.
   */
  NS_ABORT_IF (centralFrequencyBand1 > 100e9);

  /*
   * If the logging variable is set to true, enable the log of some components
   * through the code. The same effect can be obtained through the use
   * of the NS_LOG environment variable:
   *
   * export NS_LOG="UdpClient=level_info|prefix_time|prefix_func|prefix_node:UdpServer=..."
   *
   * Usually, the environment variable way is preferred, as it is more customizable,
   * and more expressive.
   */
  // if (logging)
  //   {
  //     LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  //     LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
  //     LogComponentEnable ("LtePdcp", LOG_LEVEL_INFO);
  //   }

  /*
   * Default values for the simulation. We are progressively removing all
   * the instances of SetDefault, but we need it for legacy code (LTE)
   */
  Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (999999999));

  /*
   * Create the scenario. In our examples, we heavily use helpers that setup
   * the gnbs and ue following a pre-defined pattern. Please have a look at the
   * GridScenarioHelper documentation to see how the nodes will be distributed.
   */
  const double gNbHeight = 10;
  const double ueHeight = 1.5;
  NodeContainer gNbNodes;
  NodeContainer ueNodes;
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  Ptr<ListPositionAllocator> bsPositionAlloc = CreateObject<ListPositionAllocator> ();
  Ptr<ListPositionAllocator> utPositionAlloc = CreateObject<ListPositionAllocator> ();
  gNbNodes.Create (1);
  ueNodes.Create (1);
  mobility.Install (gNbNodes);
  mobility.Install (ueNodes);
  bsPositionAlloc->Add (Vector (0.0, 0.0, gNbHeight));
  utPositionAlloc->Add (Vector (0.0, 30.0, ueHeight));
  mobility.SetPositionAllocator (bsPositionAlloc);
  mobility.Install (gNbNodes);

  mobility.SetPositionAllocator (utPositionAlloc);
  mobility.Install (ueNodes);
//   int64_t randomStream = 1;
//   GridScenarioHelper gridScenario;
//   gridScenario.SetRows (1);
//   gridScenario.SetColumns (gNbNum);
//   gridScenario.SetHorizontalBsDistance (5.0);
//   gridScenario.SetVerticalBsDistance (5.0);
//   gridScenario.SetBsHeight (1.5);
//   gridScenario.SetUtHeight (1.5);
//   // must be set before BS number
//   gridScenario.SetSectorization (GridScenarioHelper::SINGLE);
//   gridScenario.SetBsNumber (gNbNum);
//   gridScenario.SetUtNumber (ueNumPergNb * gNbNum);
//   gridScenario.SetScenarioHeight (3); // Create a 3x3 scenario where the UE will
//   gridScenario.SetScenarioLength (3); // be distribuited.
//   randomStream += gridScenario.AssignStreams (randomStream);
//   gridScenario.CreateScenario ();

  /*
   * Create two different NodeContainer for the different traffic type.
   * In ueLowLat we will put the UEs that will receive low-latency traffic,
   * while in ueVoice we will put the UEs that will receive the voice traffic.
   */
//   NodeContainer ueLowLatContainer, ueVoiceContainer;

//   for (uint32_t j = 0; j < gridScenario.GetUserTerminals ().GetN (); ++j)
//     {
//       Ptr<Node> ue = gridScenario.GetUserTerminals ().Get (j);
//       if (j % 2 == 0)
//         {
//           ueLowLatContainer.Add (ue);
//         }
//       else
//         {
//           ueVoiceContainer.Add (ue);
//         }
//     }

  /*
   * TODO: Add a print, or a plot, that shows the scenario.
   */

  /*
   * Setup the NR module. We create the various helpers needed for the
   * NR simulation:
   * - EpcHelper, which will setup the core network
   * - IdealBeamformingHelper, which takes care of the beamforming part
   * - NrHelper, which takes care of creating and connecting the various
   * part of the NR stack
   */
  Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper> ();
  //epcHelper->SetAttribute("S1uLinkDataRate", StringValue("10Gb/s"));
  epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(0)));
  Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
  Ptr<NrHelper> nrHelper = CreateObject<NrHelper> ();

  // Put the pointers inside nrHelper
  nrHelper->SetBeamformingHelper (idealBeamformingHelper);
  nrHelper->SetEpcHelper (epcHelper);

  /*
   * Spectrum division. We create two operational bands, each of them containing
   * one component carrier, and each CC containing a single bandwidth part
   * centered at the frequency specified by the input parameters.
   * Each spectrum part length is, as well, specified by the input parameters.
   * Both operational bands will use the StreetCanyon channel modeling.
   */
  BandwidthPartInfoPtrVector allBwps;
  CcBwpCreator ccBwpCreator;
  const uint8_t numCcPerBand = 1;  // in this example, both bands have a single CC

  // Create the configuration for the CcBwpHelper. SimpleOperationBandConf creates
  // a single BWP per CC
  CcBwpCreator::SimpleOperationBandConf bandConf1 (centralFrequencyBand1, bandwidthBand1, numCcPerBand, BandwidthPartInfo::UMi_StreetCanyon);
  //CcBwpCreator::SimpleOperationBandConf bandConf2 (centralFrequencyBand2, bandwidthBand2, numCcPerBand, BandwidthPartInfo::UMi_StreetCanyon);

  // By using the configuration created, it is time to make the operation bands
  OperationBandInfo band1 = ccBwpCreator.CreateOperationBandContiguousCc (bandConf1);
  //OperationBandInfo band2 = ccBwpCreator.CreateOperationBandContiguousCc (bandConf2);

  /*
   * The configured spectrum division is:
   * ------------Band1--------------|--------------Band2-----------------
   * ------------CC1----------------|--------------CC2-------------------
   * ------------BWP1---------------|--------------BWP2------------------
   */

  /*
   * Attributes of ThreeGppChannelModel still cannot be set in our way.
   * TODO: Coordinate with Tommaso
   */
  Config::SetDefault ("ns3::ThreeGppChannelModel::UpdatePeriod",TimeValue (MilliSeconds (0)));
  nrHelper->SetChannelConditionModelAttribute ("UpdatePeriod", TimeValue (MilliSeconds (0)));
  nrHelper->SetPathlossAttribute ("ShadowingEnabled", BooleanValue (false));

  /*
   * Initialize channel and pathloss, plus other things inside band1. If needed,
   * the band configuration can be done manually, but we leave it for more
   * sophisticated examples. For the moment, this method will take care
   * of all the spectrum initialization needs.
   */
  nrHelper->InitializeOperationBand (&band1);

  /*
   * Start to account for the bandwidth used by the example, as well as
   * the total power that has to be divided among the BWPs.
   */
  double x = pow (10, totalTxPower / 10);
  double totalBandwidth = bandwidthBand1;

  /*
   * if not single band simulation, initialize and setup power in the second band
   */

    allBwps = CcBwpCreator::GetAllBwps ({band1});

  /*
   * allBwps contains all the spectrum configuration needed for the nrHelper.
   *
   * Now, we can setup the attributes. We can have three kind of attributes:
   * (i) parameters that are valid for all the bandwidth parts and applies to
   * all nodes, (ii) parameters that are valid for all the bandwidth parts
   * and applies to some node only, and (iii) parameters that are different for
   * every bandwidth parts. The approach is:
   *
   * - for (i): Configure the attribute through the helper, and then install;
   * - for (ii): Configure the attribute through the helper, and then install
   * for the first set of nodes. Then, change the attribute through the helper,
   * and install again;
   * - for (iii): Install, and then configure the attributes by retrieving
   * the pointer needed, and calling "SetAttribute" on top of such pointer.
   *
   */

  Packet::EnableChecking ();
  Packet::EnablePrinting ();

  /*
   *  Case (i): Attributes valid for all the nodes
   */
  // Beamforming method
  idealBeamformingHelper->SetAttribute ("BeamformingMethod", TypeIdValue (DirectPathBeamforming::GetTypeId ()));

  // Core latency
  epcHelper->SetAttribute ("S1uLinkDelay", TimeValue (MilliSeconds (1)));

  // Antennas for all the UEs
  nrHelper->SetUeAntennaAttribute ("NumRows", UintegerValue (2));
  nrHelper->SetUeAntennaAttribute ("NumColumns", UintegerValue (4));
  nrHelper->SetUeAntennaAttribute ("AntennaElement", PointerValue (CreateObject<IsotropicAntennaModel> ()));

  // Antennas for all the gNbs
  nrHelper->SetGnbAntennaAttribute ("NumRows", UintegerValue (4));
  nrHelper->SetGnbAntennaAttribute ("NumColumns", UintegerValue (8));
  nrHelper->SetGnbAntennaAttribute ("AntennaElement", PointerValue (CreateObject<IsotropicAntennaModel> ()));

  //uint32_t bwpIdForLowLat = 0;
  uint32_t bwpIdForVoice = 0;

  // gNb routing between Bearer and bandwidh part
  //nrHelper->SetGnbBwpManagerAlgorithmAttribute ("NGBR_LOW_LAT_EMBB", UintegerValue (bwpIdForLowLat));
  nrHelper->SetGnbBwpManagerAlgorithmAttribute ("GBR_CONV_VOICE", UintegerValue (bwpIdForVoice));

  // Ue routing between Bearer and bandwidth part
  //nrHelper->SetUeBwpManagerAlgorithmAttribute ("NGBR_LOW_LAT_EMBB", UintegerValue (bwpIdForLowLat));
  nrHelper->SetUeBwpManagerAlgorithmAttribute ("GBR_CONV_VOICE", UintegerValue (bwpIdForVoice));


  /*
   * We miss many other parameters. By default, not configuring them is equivalent
   * to use the default values. Please, have a look at the documentation to see
   * what are the default values for all the attributes you are not seeing here.
   */

  /*
   * Case (ii): Attributes valid for a subset of the nodes
   */

  // NOT PRESENT IN THIS SIMPLE EXAMPLE

  /*
   * We have configured the attributes we needed. Now, install and get the pointers
   * to the NetDevices, which contains all the NR stack:
   */

  NetDeviceContainer gnbNetDev = nrHelper->InstallGnbDevice (gNbNodes, allBwps);
  //NetDeviceContainer ueLowLatNetDev = nrHelper->InstallUeDevice (ueLowLatContainer, allBwps);
  NetDeviceContainer ueNetDev = nrHelper->InstallUeDevice (ueNodes, allBwps);

  randomStream += nrHelper->AssignStreams (gnbNetDev, randomStream);
  //randomStream += nrHelper->AssignStreams (ueLowLatNetDev, randomStream);
  randomStream += nrHelper->AssignStreams (ueNetDev, randomStream);
  /*
   * Case (iii): Go node for node and change the attributes we have to setup
   * per-node.
   */

  // Get the first netdevice (enbNetDev.Get (0)) and the first bandwidth part (0)
  // and set the attribute.
  nrHelper->GetGnbPhy (gnbNetDev.Get (0), 0)->SetAttribute ("Numerology", UintegerValue (numerologyBwp1));
  nrHelper->GetGnbPhy (gnbNetDev.Get (0), 0)->SetAttribute ("TxPower", DoubleValue (10 * log10 ((bandwidthBand1 / totalBandwidth) * x)));

  // When all the configuration is done, explicitly call UpdateConfig ()

  for (auto it = gnbNetDev.Begin (); it != gnbNetDev.End (); ++it)
    {
      DynamicCast<NrGnbNetDevice> (*it)->UpdateConfig ();
    }

//   for (auto it = ueLowLatNetDev.Begin (); it != ueLowLatNetDev.End (); ++it)
//     {
//       DynamicCast<NrUeNetDevice> (*it)->UpdateConfig ();
//     }

  for (auto it = ueNetDev.Begin (); it != ueNetDev.End (); ++it)
    {
      DynamicCast<NrUeNetDevice> (*it)->UpdateConfig ();
    }

  // From here, it is standard NS3. In the future, we will create helpers
  // for this part as well.

  // create the internet and install the IP stack on the UEs
  // get SGW/PGW and create a single RemoteHost
  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // connect a remoteHost to pgw. Setup routing too
  
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("10Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (2500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.00)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);


  Ipv4AddressHelper ipv4h;
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  internet.Install (ueNodes);
  //uint64_t queueDiscSize = 1;
  // TrafficControlHelper tchCoDel;
  // //tchCoDel.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  // //Config::SetDefault ("ns3::CoDelQueueDisc::MaxSize", StringValue (std::to_string(queueDiscSize) + "p"));
  // tchCoDel.Install(gnbNetDev);
  // Config::SetDefault ("ns3::CoDelQueueDisc::MinBytes", UintegerValue (1));
  // Config::SetDefault ("ns3::CoDelQueueDisc::Target", StringValue ("0.01ms"));
 


  //Ipv4InterfaceContainer ueLowLatIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLowLatNetDev));
  Ipv4InterfaceContainer ueVoiceIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueNetDev));

  // Set the default gateway for the UEs
  for (uint32_t j = 0; j < ueNodes.GetN(); ++j)
    {
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNodes.Get (j)->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

  // attach UEs to the closest eNB
 // nrHelper->AttachToClosestEnb (ueLowLatNetDev, enbNetDev);
  nrHelper->AttachToClosestEnb (ueNetDev, gnbNetDev);

  /*
   * Traffic part. Install two kind of traffic: low-latency and voice, each
   * identified by a particular source port.
   */
  //uint16_t dlPortLowLat = 1234;
  uint16_t dlPortVoice = 1235;

  ApplicationContainer serverApps;

  // The sink will always listen to the specified ports
  //UdpServerHelper dlPacketSinkLowLat (dlPortLowLat);
  //UdpServerHelper dlPacketSinkVoice (dlPortVoice);
  uint16_t port = 50000;
  if(scenario == 1){
    
    AddressValue remoteAddress (InetSocketAddress (ueVoiceIpIface.GetAddress(0), port));
    // Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000));
    BulkSendHelper ftp ("ns3::TcpSocketFactory", Address ());
    ftp.SetAttribute ("Remote", remoteAddress);
    ftp.SetAttribute ("SendSize", UintegerValue (10000));
    ftp.SetAttribute ("MaxBytes", UintegerValue (0xFFFFFFFF));

    //uint16_t port2 = 50002;
    OnOffHelper onoff ("ns3::TcpSocketFactory", Address(InetSocketAddress (ueVoiceIpIface.GetAddress(0), port)));
    //onoff.SetAttribute ("Remote", AddressValue (ueVoiceIpIface.GetAddress(0)));
    onoff.SetConstantRate (DataRate ("1Mb/s"));
   

    serverApps.Add(ftp.Install (remoteHost));
    serverApps.Add(onoff.Install (remoteHost));
  }
  else{
    OnOffHelper onoff ("ns3::TcpSocketFactory", Address(InetSocketAddress (ueVoiceIpIface.GetAddress(0), port)));
    //onoff.SetAttribute ("Remote", AddressValue (ueVoiceIpIface.GetAddress(0)));
    onoff.SetConstantRate (DataRate ("1Mb/s"));
    serverApps.Add(onoff.Install (remoteHost));
  }
  // BulkSendHelper ftp2 ("ns3::TcpSocketFactory", Address ());
  // ftp2.SetAttribute ("Remote", remoteAddress);
  // ftp2.SetAttribute ("SendSize", UintegerValue (1000));
  // ftp2.SetAttribute ("MaxBytes", UintegerValue (0xFFFFFFFF));
  //ftp.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 5)));

  Address sinkLocalAddress (InetSocketAddress (ueVoiceIpIface.GetAddress(0), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  

  // The server, that is the application which is listening, is installed in the UE
  //serverApps.Add (dlPacketSinkLowLat.Install (ueLowLatContainer));
  //serverApps.Add (dlPacketSinkVoice.Install (ueNodes));
  

  /*
   * Configure attributes for the different generators, using user-provided
   * parameters for generating a CBR traffic
   *
   * Low-Latency configuration and object creation:
   */
//   UdpClientHelper dlClientLowLat;
//   dlClientLowLat.SetAttribute ("RemotePort", UintegerValue (dlPortLowLat));
//   dlClientLowLat.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
//   dlClientLowLat.SetAttribute ("PacketSize", UintegerValue (udpPacketSizeULL));
//   dlClientLowLat.SetAttribute ("Interval", TimeValue (Seconds (1.0 / lambdaULL)));

//   // The bearer that will carry low latency traffic
//   EpsBearer lowLatBearer (EpsBearer::NGBR_LOW_LAT_EMBB);

//   // The filter for the low-latency traffic
//   Ptr<EpcTft> lowLatTft = Create<EpcTft> ();
//   EpcTft::PacketFilter dlpfLowLat;
//   dlpfLowLat.localPortStart = dlPortLowLat;
//   dlpfLowLat.localPortEnd = dlPortLowLat;
//   lowLatTft->Add (dlpfLowLat);


  // Voice configuration and object creation:
  // UdpClientHelper dlClientVoice;
  // dlClientVoice.SetAttribute ("RemotePort", UintegerValue (dlPortVoice));
  // dlClientVoice.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
  // dlClientVoice.SetAttribute ("PacketSize", UintegerValue (udpPacketSizeBe));
  // dlClientVoice.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 5)));

  // The bearer that will carry voice traffic
  EpsBearer voiceBearer (EpsBearer::GBR_CONV_VOICE);

  // The filter for the voice traffic
  Ptr<EpcTft> voiceTft = Create<EpcTft> ();
  EpcTft::PacketFilter dlpfVoice;
  dlpfVoice.localPortStart = dlPortVoice;
  dlpfVoice.localPortEnd = dlPortVoice;

  voiceTft->Add (dlpfVoice);

  // UdpClientHelper dlClientVoice2;
  // dlClientVoice2.SetAttribute ("RemotePort", UintegerValue (dlPortVoice));
  // dlClientVoice2.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
  // dlClientVoice2.SetAttribute ("PacketSize", UintegerValue (udpPacketSizeBe));
  // dlClientVoice2.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 5)));

  /*
   * Let's install the applications!
   */
  //ApplicationContainer clientApps;
  //sinkHelper.SetAttribute ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));
  //sinkHelper.SetAttribute ("RemotePort", UintegerValue (5000));
  // sinkHelper.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
  // sinkHelper.SetAttribute ("PacketSize", UintegerValue (1000));
  // sinkHelper.SetAttribute ("Interval", TimeValue (Seconds (1.0 / 5)));
  //sinkHelper.SetAttribute ("RemoteAddress", AddressValue (internetIpIfaces.GetAddress(1)));
  ApplicationContainer clientApps;

//   for (uint32_t i = 0; i < ueLowLatContainer.GetN (); ++i)
//     {
//       Ptr<Node> ue = ueLowLatContainer.Get (i);
//       Ptr<NetDevice> ueDevice = ueLowLatNetDev.Get (i);
//       Address ueAddress = ueLowLatIpIface.GetAddress (i);

//       // The client, who is transmitting, is installed in the remote host,
//       // with destination address set to the address of the UE
//       dlClientLowLat.SetAttribute ("RemoteAddress", AddressValue (ueAddress));
//       clientApps.Add (dlClientLowLat.Install (remoteHost));

//       // Activate a dedicated bearer for the traffic type
//       nrHelper->ActivateDedicatedEpsBearer (ueDevice, lowLatBearer, lowLatTft);
//     }

  for (uint32_t i = 0; i < ueNodes.GetN (); ++i)
    {
      Ptr<Node> ue = ueNodes.Get (i);
      Ptr<NetDevice> ueDevice = ueNetDev.Get (i);
      //Address ueAddress = ueVoiceIpIface.GetAddress (i);

      // The client, who is transmitting, is installed in the remote host,
      // with destination address set to the address of the UE
      //dlClientVoice.SetAttribute ("RemoteAddress", AddressValue (internetIpIfaces.GetAddress(1)));
      //dlClientVoice2.SetAttribute ("RemoteAddress", AddressValue (internetIpIfaces.GetAddress(1)));
      //clientApps.Add (sinkHelper.Install ());
      clientApps.Add (sinkHelper.Install (ue));
      //clientApps.Add (sinkHelper.Install (remoteHost));
      // Activate a dedicated bearer for the traffic type
      nrHelper->ActivateDedicatedEpsBearer (ueDevice, voiceBearer, voiceTft);
    }

  // start UDP server and client apps
  serverApps.Start (udpAppStartTime);
  clientApps.Start (udpAppStartTime);
  serverApps.Stop (simTime);
  clientApps.Stop (simTime);

  // enable the traces provided by the nr module
  //nrHelper->EnableTraces();


  FlowMonitorHelper flowmonHelper;
  NodeContainer endpointNodes;
  endpointNodes.Add (remoteHost);
  endpointNodes.Add (ueNodes);

  Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install (endpointNodes);
  monitor->SetAttribute ("DelayBinWidth", DoubleValue (0.001));
  monitor->SetAttribute ("JitterBinWidth", DoubleValue (0.001));
  monitor->SetAttribute ("PacketSizeBinWidth", DoubleValue (20));
  
  //Ptr<TrafficControlLayer> tc = DynamicCast<NrGnbNetDevice>(gnbNetDev.Get(0))->GetNode ()->GetObject<TrafficControlLayer> ();
  //tc->GetRootQueueDiscOnDevice(gnbNetDev.Get(0));
  //Simulator::Schedule(Seconds(0.5), &func, gnbNetDev.Get(0), ueNetDev.Get(0));

  Simulator::Stop (simTime);
  //Simulator::Schedule(Seconds(0.5),&func,DynamicCast<NrGnbNetDevice>(gnbNetDev.Get(0)));
  Simulator::Run ();

  /*
   * To check what was installed in the memory, i.e., BWPs of eNb Device, and its configuration.
   * Example is: Node 1 -> Device 0 -> BandwidthPartMap -> {0,1} BWPs -> NrGnbPhy -> Numerology,
  GtkConfigStore config;
  config.ConfigureAttributes ();
  */

  // Print per-flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();

  double averageFlowThroughput = 0.0;
  double averageFlowDelay = 0.0;

  std::ofstream outFile;
  std::string filename = outputDir + "/" + simTag;
  outFile.open (filename.c_str (), std::ofstream::out | std::ofstream::trunc);
  if (!outFile.is_open ())
    {
      std::cerr << "Can't open file " << filename << std::endl;
      return 1;
    }

  outFile.setf (std::ios_base::fixed);

  double flowDuration = (simTime - udpAppStartTime).GetSeconds ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::stringstream protoStream;
      protoStream << (uint16_t) t.protocol;
      if (t.protocol == 6)
        {
          protoStream.str ("TCP");
        }
      if (t.protocol == 17)
        {
          protoStream.str ("UDP");
        }
      outFile << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> " << t.destinationAddress << ":" << t.destinationPort << ") proto " << protoStream.str () << "\n";
      outFile << "  Tx Packets: " << i->second.txPackets << "\n";
      outFile << "  Tx Bytes:   " << i->second.txBytes << "\n";
      outFile << "  TxOffered:  " << i->second.txBytes * 8.0 / flowDuration / 1000.0 / 1000.0  << " Mbps\n";
      outFile << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      if (i->second.rxPackets > 0)
        {
          // Measure the duration of the flow from receiver's perspective
          averageFlowThroughput += i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000;
          averageFlowDelay += 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets;

          outFile << "  Throughput: " << i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000  << " Mbps\n";
          outFile << "  Mean delay:  " << 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets << " ms\n";
          //outFile << "  Mean upt:  " << i->second.uptSum / i->second.rxPackets / 1000/1000 << " Mbps \n";
          outFile << "  Mean jitter:  " << 1000 * i->second.jitterSum.GetSeconds () / i->second.rxPackets  << " ms\n";
        }
      else
        {
          outFile << "  Throughput:  0 Mbps\n";
          outFile << "  Mean delay:  0 ms\n";
          outFile << "  Mean jitter: 0 ms\n";
        }
      outFile << "  Rx Packets: " << i->second.rxPackets << "\n";
    }

  outFile << "\n\n  Mean flow throughput: " << averageFlowThroughput / stats.size () << "\n";
  outFile << "  Mean flow delay: " << averageFlowDelay / stats.size () << "\n";

  outFile.close ();

  std::ifstream f (filename.c_str ());

  if (f.is_open ())
    {
      std::cout << f.rdbuf ();
    }

  Simulator::Destroy ();
  return 0;
}


