#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x32e21920, "module_layout" },
	{ 0x2d91d906, "seq_lseek" },
	{ 0x26cf5fb, "remove_proc_entry" },
	{ 0x999e8297, "vfree" },
	{ 0x4a3c27d2, "proc_create" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x29126269, "seq_printf" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x57a4c8d6, "seq_open" },
	{ 0xd62ca47, "seq_read" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xc486a127, "seq_release" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DB76F032594DBC7C634AF7F");
