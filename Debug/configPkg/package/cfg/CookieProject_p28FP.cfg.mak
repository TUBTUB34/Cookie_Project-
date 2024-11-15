# invoke SourceDir generated makefile for CookieProject.p28FP
CookieProject.p28FP: .libraries,CookieProject.p28FP
.libraries,CookieProject.p28FP: package/cfg/CookieProject_p28FP.xdl
	$(MAKE) -f C:\7820\CookieProject/src/makefile.libs

clean::
	$(MAKE) -f C:\7820\CookieProject/src/makefile.libs clean

