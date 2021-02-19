from xmlrpc.server import SimpleXMLRPCServer
import xmlrpc.client
import os

server = SimpleXMLRPCServer(("0.0.0.0", 8000))

def Send2Client():
    with open("pipe.txt", "rb") as handle:
        return xmlrpc.client.Binary(handle.read()) ## send results to client

def UploadFile(filedata):
    try:
        fp = "serv-test.xml"
        with open(fp, "wb") as handle: ## open file for writing test
            data1=filedata.data
            handle.write(data1) ## write in file
            handle.close() ## close file
        return True
    except Exception:
        return 'error'


def ExecFile():
    fp = "serv-test.xml"
    os.system("./ASVK_Main_Task1 " + fp + " > pipe.txt")  ## execute with received test
    return True

if os.path.exists("pipe.txt"):
    os.remove("pipe.txt") ## remove pipe.txt
server.register_function(UploadFile) ## wait for UploadFile to be called
server.register_function(ExecFile) ## wait for ExecFile to be called
server.register_function(Send2Client) ## wait for Send2Client to be called

try:
    print(" Use Control-C to exit ")
    server.serve_forever() ## work until Ctrl+C
except KeyboardInterrupt:
    print(" Exiting ")
