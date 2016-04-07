WSLauncher
==================

Do you have too much RAM?
Do you wish for the old days when individual programs consumed significant portions of your total memory size?
Well good news! This utility will let you arbitrarily reserve vast tracts of non-paged memory to a process that doesn't even exist yet. So easy to use you'll probably crash your machine in no time. Actually, I've put in a limit of 1G to (probably?) prevent that.

This program operates as a shim launcher, the utility creates the process you want then fiddles with the working-set reservation for you before the process actually executes any instructions (it is launched suspended so it never sees anything but what you specify). Provide the number of physical pages to reserve on the command-line and then every argument after that is assumed to be the command-line of the process to launch. Alternately you can append the size parameter with K or M to specify the size in Kilobytes or Megabytes respectively.

The releases have no dependencies.
