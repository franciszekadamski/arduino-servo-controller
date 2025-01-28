import rpyc
c = rpyc.connect("localhost", 18812)
c.root.echo("hello")
