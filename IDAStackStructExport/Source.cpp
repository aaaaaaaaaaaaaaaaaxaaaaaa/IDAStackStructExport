/*
 *  This is a sample plugin module
 *
 *  It can be compiled by any of the supported compilers:
 *
 *      - Borland C++, CBuilder, free C++
 *      - Visual C++
 *      - GCC
 *
 */
#pragma warning(push, 0)        
#include <ida.hpp>
#include <idp.hpp>
#include <expr.hpp>
#include <bytes.hpp>
#include <loader.hpp>
#include <kernwin.hpp>
#include <frame.hpp>
#include <struct.hpp>
#include <name.hpp>
#include <cstdio>
#pragma warning(pop)

int idaapi init(void)
{
	return PLUGIN_OK;
}
void idaapi term(void)
{

}

void trim_string(char* str) {

}

bool idaapi run(size_t arg)
{
	/* Create new container structure */
	auto ea = get_screen_ea();
	qstring function_name;
	get_func_name(&function_name, ea);
	char generated_name[256];
	qsnprintf(generated_name, 256, "StackFrame_%X", ea);
	auto generated_struc = get_struc(add_struc(BADADDR, generated_name, false));

	auto frame = get_frame(ea);
	member_t* members = frame->members;
	auto member_count = frame->memqty;
	for (uint32 i = 0; i < member_count; i++) {
		member_t member = members[i];
		auto name = get_member_name(member.id);
		auto start = member.soff;
		auto end = member.eoff;
		auto size = end - start;
		opinfo_t opinfo;
		retrieve_member_info(&opinfo, &member);

		auto result = add_struc_member(generated_struc, name.trim2(' ').c_str(), member.soff, member.flag, &opinfo, size);
		if (result != STRUC_ERROR_MEMBER_OK) {
			switch (result) {
			case STRUC_ERROR_MEMBER_NAME: msg("STRUC_ERROR_MEMBER_NAME\n"); break;
			case STRUC_ERROR_MEMBER_NESTED: msg("STRUC_ERROR_MEMBER_NESTED\n"); break;
			case STRUC_ERROR_MEMBER_OFFSET: msg("STRUC_ERROR_MEMBER_OFFSET\n"); break;
			case STRUC_ERROR_MEMBER_SIZE: msg("STRUC_ERROR_MEMBER_SIZE\n"); break;
			case STRUC_ERROR_MEMBER_STRUCT: msg("STRUC_ERROR_MEMBER_STRUCT\n"); break; 
			case STRUC_ERROR_MEMBER_TINFO: msg("STRUC_ERROR_MEMBER_TINFO\n"); break;
			case STRUC_ERROR_MEMBER_UNIVAR: msg("STRUC_ERROR_MEMBER_UNIVAR\n"); break;
			case STRUC_ERROR_MEMBER_VARLAST: msg("STRUC_ERROR_MEMBER_VARLAST\n"); break;
			}
		}
	}




	//msg("just fyi: the current screen address is: %a\n", get_screen_ea());
	return true;
}

//--------------------------------------------------------------------------
static const char comment[] = "Copies a stack frame into a local structure";

static const char help[] = "";

//--------------------------------------------------------------------------
// This is the preferred name of the plugin module in the menu system
// The preferred name may be overriden in plugins.cfg file

static const char wanted_name[] = "Copy stack frame to a local structure";


// This is the preferred hotkey for the plugin module
// The preferred hotkey may be overriden in plugins.cfg file
// Note: IDA won't tell you if the hotkey is not correct
//       It will just disable the hotkey.

static const char wanted_hotkey[] = "";


//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  PLUGIN_UNL,           // plugin flags
  init,                 // initialize

  term,                 // terminate. this pointer may be NULL.

  run,                  // invoke plugin

  comment,              // long comment about the plugin
						// it could appear in the status line
						// or as a hint

  help,                 // multiline help about the plugin

  wanted_name,          // the preferred short name of the plugin
  wanted_hotkey         // the preferred hotkey to run the plugin
};
