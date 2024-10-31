# Toralize

**Toralize** is a command-line utility that enhances application privacy by proxying requests through the Tor network using SOCKS version 4. This tool requires the Tor software to be installed and configured on your system.

## Usage
To use Toralize, simply prefix your command with toralize to proxy it through the Tor network.
```bash
$ toralize curl www.google.com
```
## Prerequisites

- **Tor**: Make sure Tor is installed and running on your system.
- Download and compile the project.
- Move the bash script to your bin directory and point LD_EXPORT to the compiled binary's location.
- Change the proxy port in toralizer.h to match your Tor port (default is 9050).

## Note
Some servers may block or limit access to requests coming from the Tor network.
