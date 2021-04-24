SECTIONS
{
	.init: { boot*(.text)} > 0x80000000 /* make sure we can boot!            */	
}