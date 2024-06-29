#include <iostream>
#include <string>
#include <unistd.h>

#include <lldb/API/LLDB.h>

bool check_running_with_root() {
    auto uid = getuid();
    if (uid != 0) {
        std::cerr << "You must be run as root\n";
        return false;
    }
    return true;
}

const char *const GET_OS_ELIGIBILITY_DOMAIN_XCODE_LLM_COMMAND
    = "po [[[[EligibilityEngine sharedInstance] domains] "
      "objectForKey:@\"OS_ELIGIBILITY_DOMAIN_XCODE_LLM\"] answer]";
const char *const RECOMPUTE_ALL_DOMAIN_ANSWERS_COMMAND
    = "e (void) [[EligibilityEngine sharedInstance] recomputeAllDomainAnswers]";
const char *const MODIFY_DEVICE_REGION_CODE_COMMAND
    = "e (void) [[[InputManager sharedInstance] objectForInputValue:6] "
      "setValue:@\"US\" forKey:@\"_deviceRegionCode\"]";
const int UNSUPPORTED_DOMAIN_CODE = 2;

int hex_to_int(const std::string &hex) {
    return std::stoi(hex, nullptr, 16);
}

#define CLEAN_RETURN(code) \
    process.Detach(); \
    lldb::SBDebugger::Terminate(); \
    return code

int main(int argc, char **argv) {
    if (!check_running_with_root()) {
        return 1;
    }

    lldb::SBDebugger::Initialize();
    auto debugger = lldb::SBDebugger::Create();
    debugger.SetAsync(false);

    auto target = debugger.CreateTarget("");
    lldb::SBError error;
    auto listener = debugger.GetListener();

    std::cout << "[*] Attaching to process eligibilityd\n";
    auto process = target.AttachToProcessWithName(listener, "eligibilityd", false, error);
    if (error.Fail() || !process.IsValid()) {
        std::cerr << "Failed to attach to process: ", error.GetCString();
        CLEAN_RETURN(2);
    }
    std::cout << "[*] Process attached\n";

    auto interpreter = debugger.GetCommandInterpreter();
    lldb::SBCommandReturnObject result;
    interpreter.HandleCommand(GET_OS_ELIGIBILITY_DOMAIN_XCODE_LLM_COMMAND, result);
    if (!result.HasResult()) {
        std::cerr << "Failed to get OS_ELIGIBILITY_DOMAIN_XCODE_LLM\n";
        CLEAN_RETURN(3);
    }
    auto domain_code = hex_to_int(result.GetOutput());
    if (domain_code != UNSUPPORTED_DOMAIN_CODE) {
        std::cerr << "Xcode predictive code completion is now available\n";
        CLEAN_RETURN(4);
    }

    std::cout << "[*] Modifying device region code\n";
    interpreter.HandleCommand(MODIFY_DEVICE_REGION_CODE_COMMAND, result);

    std::cout << "[*] Recomputing all device domain answers\n";
    interpreter.HandleCommand(RECOMPUTE_ALL_DOMAIN_ANSWERS_COMMAND, result);

    // Check if the domain code is changed.
    interpreter.HandleCommand(GET_OS_ELIGIBILITY_DOMAIN_XCODE_LLM_COMMAND, result);
    domain_code = hex_to_int(result.GetOutput());
    if (domain_code != 4) {
        std::cerr << "Failed to enable Xcode predictive code completion\n";
        CLEAN_RETURN(5);
    }

    std::cout << "Enjoy the Xcode predictive code completion\n";

    error.Clear();

    CLEAN_RETURN(0);
}