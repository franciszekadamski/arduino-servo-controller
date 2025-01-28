import rpyc
from rpyc.utils.server import ThreadedServer

class MyService(rpyc.Service):
    # My service
    def exposed_echo(self, text):
        print(text)

if __name__ == "__main__":
    server = ThreadedServer(MyService, port = 18812)
    server.start()
