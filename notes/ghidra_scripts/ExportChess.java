// Ghidra headless post-script (Java): dump a full disassembly listing and,
// for every defined function, the decompiler's C-like pseudocode.
//
// Output files:
//   chess_disasm.txt   - address / bytes / mnemonic+operands / EOL comment
//   chess_decompiled.c - one section per function, decompiler pseudocode

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;

import java.io.PrintWriter;

public class ExportChess extends GhidraScript {

    static final String OUT_DIR =
        "C:\\Users\\talne\\Desktop\\ClaudeCodePicoCalc\\chess-reverse\\disasm\\";

    @Override
    public void run() throws Exception {
        dumpListing();
        dumpDecompiled();
    }

    void dumpListing() throws Exception {
        Listing listing = currentProgram.getListing();
        PrintWriter out = new PrintWriter(OUT_DIR + "chess_disasm.txt", "UTF-8");
        InstructionIterator it = listing.getInstructions(true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            Address addr = ins.getAddress();
            byte[] b = ins.getBytes();
            StringBuilder hexbytes = new StringBuilder();
            for (byte x : b) {
                hexbytes.append(String.format("%02x ", x));
            }
            String eol = listing.getComment(CodeUnit.EOL_COMMENT, addr);
            String line = String.format("%-12s %-24s %s", addr, hexbytes.toString().trim(),
                ins.toString());
            if (eol != null) {
                line += "    ; " + eol.replace("\n", " / ");
            }
            out.println(line);
        }
        out.close();
        println("wrote " + OUT_DIR + "chess_disasm.txt");
    }

    void dumpDecompiled() throws Exception {
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(OUT_DIR + "chess_decompiled.c", "UTF-8");
        FunctionIterator funcs = currentProgram.getFunctionManager().getFunctions(true);
        int count = 0;
        while (funcs.hasNext()) {
            Function f = funcs.next();
            count++;
            out.println("// ---- function " + f.getName() + " at " + f.getEntryPoint() + " ----");
            DecompileResults res = ifc.decompileFunction(f, 60, monitor);
            if (res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed: " + res.getErrorMessage());
            }
            out.println();
        }
        out.close();
        println("wrote " + OUT_DIR + "chess_decompiled.c (" + count + " functions)");
    }
}
