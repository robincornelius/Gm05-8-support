
GMControl2ps.dll: dlldata.obj GMControl2_p.obj GMControl2_i.obj
	link /dll /out:GMControl2ps.dll /def:GMControl2ps.def /entry:DllMain dlldata.obj GMControl2_p.obj GMControl2_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del GMControl2ps.dll
	@del GMControl2ps.lib
	@del GMControl2ps.exp
	@del dlldata.obj
	@del GMControl2_p.obj
	@del GMControl2_i.obj
