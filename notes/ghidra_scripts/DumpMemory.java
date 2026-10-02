import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.address.Address;

public class DumpMemory extends GhidraScript {
    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        Address base = currentProgram.getAddressFactory().getAddress("10ff:0a8c");
        byte[] buf = new byte[1600];
        mem.getBytes(base, buf);
        StringBuilder hex = new StringBuilder();
        for (int i = 0; i < buf.length; i++) {
            hex.append(String.format("%02x", buf[i]));
            if (i % 16 == 15) hex.append("\n"); else hex.append(" ");
        }
        println("GLYPHDATA_START");
        println(hex.toString());
        println("GLYPHDATA_END");
    }
}
