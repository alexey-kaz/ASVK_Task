import xmlrpc.client
import os
import glob
import sys

if len(sys.argv) == 2 :
    adr = sys.argv[1]

    files = []

    proxy= xmlrpc.client.ServerProxy("http://" + adr + ":8000/")

    for file in glob.glob("*.xml"):
        files.append(file) ## find all .xml files in directory

    if 'serv-test.xml' in files :
        files.remove("serv-test.xml") ## delete serv-test.xml from list

    if os.path.exists("pipe.txt"):
        os.remove("pipe.txt")  ## delele pipe.txt
        
    for x in files:
        filepath = x
        print('\n' + x) ## to let us know which test is running
        with open(filepath, "rb") as handle:
            data = xmlrpc.client.Binary(handle.read()) ## read test into data as binary
            handle.close() ## close the test file, info is still in data
            proxy.UploadFile(data) ## send test
            proxy.ExecFile() ## execute program with sent test
            print(proxy.Send2Client()) ## client receives results
else :
    print("argv error")

