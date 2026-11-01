#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xdc352a3b, "__list_add_valid_or_report" },
	{ 0xd710adbf, "__kmalloc_noprof" },
	{ 0x43ade889, "proc_create" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x5e505530, "kthread_should_stop" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe16f6e65, "wake_up_process" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xc013f05a, "STUB_stop_elevator" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0xf46d5bf3, "mutex_lock" },
	{ 0x4f1e5fd0, "__list_del_entry_valid_or_report" },
	{ 0xe4fd88b3, "kthread_stop" },
	{ 0xc1e6c71e, "__mutex_init" },
	{ 0x357aaab3, "mutex_lock_interruptible" },
	{ 0x27683a56, "memset" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x70956c7a, "STUB_issue_request" },
	{ 0x380324b6, "proc_remove" },
	{ 0x409a6943, "kthread_create_on_node" },
	{ 0xdd6830c7, "sprintf" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0x9b217dc6, "__kmalloc_cache_noprof" },
	{ 0x437e81c7, "simple_read_from_buffer" },
	{ 0x67628f51, "msleep" },
	{ 0x507d4833, "kmalloc_caches" },
	{ 0xc013f05a, "STUB_start_elevator" },
	{ 0xd040b19c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xdc352a3b,
	0xd710adbf,
	0x43ade889,
	0xcb8b6ec6,
	0x5e505530,
	0xd272d446,
	0xe16f6e65,
	0xbd03ed67,
	0xd272d446,
	0xc013f05a,
	0x90a48d82,
	0xbd03ed67,
	0xf46d5bf3,
	0x4f1e5fd0,
	0xe4fd88b3,
	0xc1e6c71e,
	0x357aaab3,
	0x27683a56,
	0xd272d446,
	0x70956c7a,
	0x380324b6,
	0x409a6943,
	0xdd6830c7,
	0xf46d5bf3,
	0x9b217dc6,
	0x437e81c7,
	0x67628f51,
	0x507d4833,
	0xc013f05a,
	0xd040b19c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__list_add_valid_or_report\0"
	"__kmalloc_noprof\0"
	"proc_create\0"
	"kfree\0"
	"kthread_should_stop\0"
	"__fentry__\0"
	"wake_up_process\0"
	"__ref_stack_chk_guard\0"
	"__stack_chk_fail\0"
	"STUB_stop_elevator\0"
	"__ubsan_handle_out_of_bounds\0"
	"random_kmalloc_seed\0"
	"mutex_lock\0"
	"__list_del_entry_valid_or_report\0"
	"kthread_stop\0"
	"__mutex_init\0"
	"mutex_lock_interruptible\0"
	"memset\0"
	"__x86_return_thunk\0"
	"STUB_issue_request\0"
	"proc_remove\0"
	"kthread_create_on_node\0"
	"sprintf\0"
	"mutex_unlock\0"
	"__kmalloc_cache_noprof\0"
	"simple_read_from_buffer\0"
	"msleep\0"
	"kmalloc_caches\0"
	"STUB_start_elevator\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "5C31AB29D7804EFF64496E5");
