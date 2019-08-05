#include<string.h> /* strchr */
#include<stdio.h> /* printf, fopen, fclose */
#include<stdlib.h> /* getenv */
#include<sys/sysinfo.h> /* uptime,freeram,totalram,procsg */
#include<sys/utsname.h> /* uname, kernel, cpu, hostname (nodename) */
#include<glob.h> /* packages */
#include<unistd.h> /* readlink */
#include<sys/stat.h> /* stat */
int main(void){
static FILE *file;
//static int prof1,prof2;
static char gcc[20],
resolution[10],
clib[33],
clibv[48],
alsa[36],
motherboard[25],
//prof[64],
//release[13],
cpul[60],
ge[12],
root[16],
*cpu;
static struct stat sock;
static struct sysinfo sys;
static struct utsname kernel;
static  glob_t packages;
//glob("/var/db/pkg/*/*",GLOB_NOSORT|GLOB_NOESCAPE,NULL,&packages);
glob("/var/lib/pacman/local/*",GLOB_NOSORT|GLOB_NOESCAPE, NULL, &packages);
sysinfo(&sys);
uname(&kernel);

/* CPU */
 file=fopen("/proc/cpuinfo","r");
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,60,file); /* jump to 5th line find a better way */
 fclose(file);
 cpu=&cpul[13];

/* gcc */
 file=popen("gcc --version","r");
 fgets(gcc,19,file);
 pclose(file);

/* root */
 file=popen("df -hTx devtmpfs -x tmpfs|awk 'FNR==2{print$4\"/\"$3\" \"$2}'","r");
 fgets(root,16,file);

/* motherboard */
  file=fopen("/sys/devices/virtual/dmi/id/product_name","r");
  fgets(motherboard,25,file);
  fclose(file);

/* clib */
// file=popen("gcc -dumpmachine","r"); takes too much time
// file=popen("/lib/libc.so.6","r"); //4characters needed.
 file=popen("gcc -dumpmachine","r");
 fgets(clib,18,file);
 pclose(file);
 switch (clib[strlen(clib)-1]) {
  case 'g':
   strcpy(clib,"glibc ");
//   file=popen("/lib/libc-*","r");
   file=popen("/lib/libc.so.6","r");
   fgets(clibv,53,file);
   pclose(file);
   strcpy(&clib[6],&clibv[48]);
  break;
  case 'u':
   strcpy(clib,"ucblibc");
  break;
  case 'm':
   strcpy(clib,"musl");
  break;
 }


/* Resolution */
 file=fopen("/sys/devices/virtual/graphics/fbcon/subsystem/fb0/device/drm/card0/device/graphics/fb0/virtual_size","r");
 fgets(resolution,10,file);
 fclose(file);
 *strchr(resolution,',')='x';

/* sound card detection */

 file=popen("aplay -L","r");
 do
  fgets(alsa,18,file);
 while (alsa[0]!='d');
 fgets(alsa,36,file);
 pclose(file);


/* graphic environment */
 stat("/tmp/dmitry-runtime-dir/wayland-0",&sock);
 if (sock.st_mode==49645)
 strcpy(ge,"Wayland");
 else {
   stat("/tmp/.X11-unix/X0",&sock);
    if (sock.st_mode==49663)
     strcpy(ge,"X11");
    else
     strcpy(ge,"Framebuffer");
 }
 if(getenv("TERM")[0]=='l')
  strcpy(ge,"Framebuffer");

printf("[1;36m                  -`                     %s[1;37m@[1;36m%s\n\
[1;36m                 .o+`                    OS:[m Arch Linux%s\n\
[1;36m                `ooo/                    MotherBoard:[m %s %s\
[1;36m              `+oooo:                   Kernel:[m %s %s\n\
[1;36m              `+oooooo:                  Clib:[m %s\n\
[1;36m              -+oooooo+:                 GCC:[m %s\
[1;36m            `/:-:++oooo+:                Graphic Environment:[m %s\n\
[1;36m           `/++++/+++++++:               Terminal:[m %s\n\
[1;36m          `/++++++++++++++:              Shell:[m %s\n\
[1;36m         `/+++o[0m[36moooooooo[0m[1;36moooo/             Packages:[m %lu\n\
[1;36m        [0m[1;36m./[0m[36mooosssso++osssssso[0m[1;36m+`           Root:[m %s\
[36m       .oossssso-````/ossssss+`          [1;36mUptime:[m %lud,%luh,%lum\n\
[36m      -osssssso.      :ssssssso.         [1;36mProcesses:[m %u\n\
[36m     :osssssss/        osssso+++.        [1;36mRAM:[m %luMiB/%luMiB\n\
[36m    /ossssssss/        +ssssooo/         [1;36mCPU:[m %s\
[36m   `/ossssso+/:-       -:/+osssso+-      [1;36mGPU:[m Intel HD Graphics 4600\n\
[36m  `+sso+:-`                `.-/+oso:     [1;36mResolution:[m %s\n\
[36m `++:.                          `-/+/    [1;36mSound Card:[m %s\n\
[36m .`                                `/[0m \n",
getenv("USER"),kernel.nodename,
"",
motherboard, "",
kernel.sysname,kernel.release,
clib,
&gcc[10],
ge,
getenv("TMUX_PANE")?"tmux":getenv("TERM"),
getenv("SHELL"),
packages.gl_pathc,
root,
sys.uptime/86400,sys.uptime/3600%24,sys.uptime/60%60,
sys.procs,
(sys.totalram-sys.freeram)/1048576,sys.totalram/1048576,
cpu,
resolution,
&alsa[4]);
//remove strings which where "Stable"
return 0;
}
