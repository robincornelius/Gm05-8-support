
HMI_timerps.dll: dlldata.obj HMI_timer_p.obj HMI_timer_i.obj
	link /dll /out:HMI_timerps.dll /def:HMI_timerps.def /entry:DllMain dlldata.obj HMI_timer_p.obj HMI_timer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del HMI_timerps.dll
	@del HMI_timerps.lib
	@del HMI_timerps.exp
	@del dlldata.obj
	@del HMI_timer_p.obj
	@del HMI_timer_i.obj
