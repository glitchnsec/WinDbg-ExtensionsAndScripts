/// <reference path="JsProvider.d.ts" />
"use strict";

// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/javascript-debugger-scripting
// intellisense: https://blogs.msdn.microsoft.com/windbg/2017/05/18/intellisense-for-debugger-javascript-apis/

// WinDbg Javascript
// Though I walk through the valley of the shadow of death,
// I fear no evil.

// Root namespace in the debugger is Debugger
// dx Debugger

/**
 * Usage:
 * .load jsprovider.dll
 * 
 */

/*****************************
 * Utilities                 *
 *****************************/
function logInfo(info)
{
    host.diagnostics.debugLog(info + '\n');
}

function toHex(data)
{
    return data.toString(16);
}

/**
 * Check if the architecture is x86
 */
function isx86()
{
    // x86 or x64?
    // Maybe s.Attributes.Machine.PointerSize is more reliable? Ha..
    return host.namespace.Debugger.Sessions.Any(s => (s.Attributes.Machine.AbbrevName == "x86"));
}

/**
 * Essential EIP for the current thread
 */
function curInstrAddress()
{
    var inst_address = isx86() ? host.currentThread.Registers.User.eip : host.currentThread.Registers.User.rip;
	return inst_address;
}

/**
 * Returns an array of the first Length instructions starting at
 * instr_address
 * @param {string} instr_address 
 * @param {int} length 
 */
function unassemble(instr_address, length)
{
    if (instr_address === undefined)
    {
        instr_address = curInstrAddress();
    }

    return host.namespace.Debugger.Utility.Code
            .CreateDisassembler()
            .DisassembleInstructions(instr_address)
            .Take(length);
}

/**
 * Attempt to find the function within
 * which the instruction at address resides.
 * 
 * Useful if you suspect the stacktrace is wrong/ambiguous
 * @param {*} inst_address 
 */
function findHostfunction(inst_address)
{
    // use instruction pointer if no address provided
    if (inst_address === undefined) {
        inst_address = curInstrAddress();
        logInfo("Using current instruction: ");
    }
    // Get the current module
    func = host.getModuleContainingSymbol(inst_address);

    // for each function within the module
    // if the given address

    // TODO: what if it doesn't reside in a function
    // is this a thing? I don't know
    return func;
}

/**
 * Compare instruction at address with instr
 * @param {Array} instr instruction bytes
 * @param {string} address address the test or current eip
 */
function CompareInstruction(instr, address){
    if (address == undefined){
        address = curInstrAddress();
    }

    var instrAtAddress = unassemble(address, 1).First();
    for (var cbyteIdx = 0; cbyteIdx < instrAtAddress.Length; cbyteIdx++)
    {
        if (instr[cbyteIdx] !== instrAtAddress.CodeBytes[cbyteIdx]) return false;
    }
    return true;
}

/**
 * For use in a TTD environment.
 * Execute backwards searching for function prologue
 * Return address of push ebp
 * @param {*} inst_address 
 */
function TTD_findHostFunction(start_time){
    // confirm we are in TTD
    const CurrentSession = host.currentSession;
    const CurrentProcess = host.currentProcess;
    const CurrentThread = host.currentThread;
    const Utility = host.namespace.Debugger.Utility;
    if (!CurrentSession.Attributes.Target.IsTTDTarget){
        logInfo("Requires TTD session");
        return;
    }

    // check if time is undefined
    if (start_time === undefined) {
        start_time=CurrentThread.TTD.Position;
    }

    // Execute backwards till
    var ctl = Utility.Control;
    
    var found = false;
    var func_time = undefined;
	var func_addr = undefined;
    var x86 = isx86();

    // mov esp, ebp
    var pre_requisite = false;

    var func_prologue = x86? [[0x55], [0x8b, 0xec]] : [[], []]; // push ebp, mov ebp, esp : push

    while (!found)
    {
		// First check to see if we are already at a function
		if (CompareInstruction(func_prologue[0], undefined)) {
			
			// check forward
			ctl.ExecuteCommand("p");
			
			// check mov ebp, esp
			if (CompareInstruction(func_prologue[1], undefined)) { 

				found = true;
				
				// reset
				ctl.ExecuteCommand("p-");

				// get the current TTD time
				func_time = CurrentThread.TTD.Position;

				// get the address of push ebp
				func_addr = curInstrAddress();
			
			}

		} else if (pre_requisite) {
            // then check for push epb
            if (CompareInstruction(func_prologue[0], undefined)) {
                found = true;

                // get the current TTD time
                func_time = CurrentThread.TTD.Position;

                // get the address of push ebp
                func_addr = curInstrAddress();


            } else {
                // reset pre_requisite
                pre_requisite = false;
            }
        // check mov ebp, esp
        } else if (CompareInstruction(func_prologue[1], undefined)) {
            pre_requisite = true;
        }
		ctl.ExecuteCommand("p-");
        
    }

    // verify if what we have identified is truly a function
    // if it is, we should be at a call instruction at this point
    if (!unassemble(undefined, 1).First().Attributes.IsCall)
    {
        logInfo("Please verify: It appears we don't have a function\n");
    }

    // Function address found, seek back to current original time
    //ctl.ExecuteCommand('dx @$create("Debugger.Models.TTD.Position",' + time[] + ', ' +time[] + ').SeekTo()' )
    start_time.SeekTo();

    
    // return
    return { funct: func_addr, position: func_time }

}

/**
 * An instruction context consists of 
 * The function within which the instruction resides
 * A current dump of the registers
 * The arguments supplied to the function
 * @param {*} inst_address 
 */
function showInstructionContext(inst_address)
{
    // get the current register values

    // get the current function
    func = findHostfunction(inst_address);

    params = enumerateFunctionParameters(func);

    regs = host.currentThread.Registers.User;

    return { function: func, parameters: parameters, registers: regs };
}

/**
 * An array of parameters passed to a function.
 * Should be same with values from Kv but more suitable
 * for immediate instruction
 * @param {*} func_address 
 */
function enumerateFunctionParameters(func_address)
{
    return func_address;
}

/**
 * 
 */


/********************************
 * TODOs                        *
 * Most of these. If implemented*
 * should be added to utilities *
 * above                        *
 ********************************/

function fixStackTrace()
{
    // for each instruction address on the stack trace,
    // call findHostfunction

}


/*****************************
 * Executes on .scriptunload *
 *****************************/
function uninitializeScript() {

}

/***************************
 * Executes on .scriptload *
 *             .scriptrun  *
 ***************************/

function initializeScript() {

}
var print_prefix = "***> Hello\n"
host.diagnostics.debugLog(print_prefix + "")