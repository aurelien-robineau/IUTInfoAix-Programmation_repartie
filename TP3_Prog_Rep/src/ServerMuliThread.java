public class ServerMuliThread {
    TaskServerEcho task = new TaskServerEcho(client);
    new Thread(task).start();
}
