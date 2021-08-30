# getaddrinfo
Drop in getaddrinfo that can be preloaded to bypass dns

## building
cc -fpic -o dummy_getaddrinfo.so -shared dummy_getaddrinfo.c -ldl

## using
export DUMMY_HOST=\<host name that you want to redirect\>

export DUMMY_IP=\<ip that you want to send it to\>
  
LD_PRELOAD=dummy_getaddrinfo.so host <host name that you want to redirect>
