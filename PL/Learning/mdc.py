#!/usr/bin/python

import sys, csv

def init_check():
    '''Opens datafile and creates fileptr to filename.cv '''
    if len(sys.argv) != 2:
        print ("Usage mdc.py datafile.csv")
        sys.exit(1)

    file = open( sys.argv[1])
    file = csv.reader(file)
    file = list(file)
    
    header = [[],[]]
    header[0] = file.pop(0)
    header[1]=  file.pop(0)  


    out = list(sys.argv[1][:-1])
    out[-1] = 'v'
    
    out = open("".join(out) , 'w')
    #print(file)
    return (file,header ,out)

def normalize( data, quant ):

    summary = []
    
    for i in range(2,2+quant):
        for j in data :
            summary.append ( j[i])
        huge = max(summary)
        tiny = min(summary)
        for j in data:
            j[i] = (float(j[i])- float(tiny)) / (float(huge) - float(tiny))
    return data

def simulate( data , SamNum , CentNum , quant ):
    '''Data is our data set
    SamNum is the sample we are seeing if we can identify
    CentNum is the amount of diffrent centroids we will produce
    quant is the dimentions of vectorspace we are working in'''

    sample = data.pop(SamNum)

    CentVector = [[] for i in range(CentNum)]
    
    for i in CentVector:
        for j in range(quant+1):
            i.append(0)

    for line in data:
        #incriment learning vector
        CentVector[ int(line[1]) ][0] += 1
        for i in range(2,quant+2):
            CentVector[ int(line[1]) ][i-1] += float(line[i])
    
  #  print (CentVector)
        
    for value in CentVector:
        for i in range(1,quant+1):
            value[i] = value[i]/value[0]
        
    least = [1000000,-1]

   # print (CentVector)

    for j in range(0 , CentNum):
        dist = 0
        for i in range(1, quant ):
    #        print ( sample[i+1], " - " ,CentVector[j][i+1])
            dist += (sample[i+1]-CentVector[j][i+1])**2
        if( dist < least[0]):
            least[0] = dist
            least[1] = j
    
    return ( SamNum, sample[1], least[1] )

def output( outfile, results, header):
    print (header[0][0])
    outfile.write(header[0][0])
    
    for i in range(1,len(header[0]) ):
        print("class", header[0][i][:1], ' (', header[0][i][2:], '):' )


def main():
    
    file,header, out  = init_check()

    params =  len(header[1])-2 
    cent = len(header[0])-1
    file = normalize ( file, params )
    

    for i in range( 0 , 150, 10):#len(file) ):
        result = simulate( file[:], i , cent, params)

        if(int(result[1]) == int(result[2]) ):
            print( result[0],',',result[1],',',result[2])
        else:
            print( result[0],',',result[1],',',result[2],'*')
        pass
    print()
    output( out, result, header)

    sys.exit()

if __name__ == "__main__":
    main()


