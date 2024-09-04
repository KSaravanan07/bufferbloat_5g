import os

if __name__ == "__main__":

    current_directory = os.getcwd()
    stats_directory = os.path.join(current_directory, r'Stats/Setup')
    if not os.path.exists(stats_directory):
        os.makedirs(stats_directory)

    final_directory = os.path.join(stats_directory, r'MiceFlow')
    if not os.path.exists(final_directory):
        os.makedirs(final_directory)
        
    print("---------------------------------Mice Flow---------------------------------")

    os.system("./ns3 run 'bufferbloat.cc --scenario=1 --outputDir=./Stats/Setup/MiceFlow'")

    final_directory = os.path.join(stats_directory, r'MiceElephantFlow')
    if not os.path.exists(final_directory):
        os.makedirs(final_directory)
        
    print("--------------------------Mice Flow, with Elephant Flow--------------------")

    os.system("./ns3 run 'bufferbloat.cc --scenario=2 --outputDir=./Stats/Setup/MiceElephantFlow'")
    
    file1 = open("./Stats/Setup/MiceFlow/default","r+")
    lines = file1.readlines()
    lines = [i.strip() for i in lines]
    
    tx = lines[6][13:18]
    print("\n\nMean Delay in Mice Flow only: ", tx, "ms")
    
    file1 = open("./Stats/Setup/MiceElephantFlow/default","r+")
    lines = file1.readlines()
    lines = [i.strip() for i in lines]
    
    tx = lines[15][13:19]
    print("\n\nMean Delay in Mice Flow with Elephant Flow introduced: ", tx, "ms")
    
    
    
    
    
