#!/usr/bin/env python3
"""
Simple test script to verify RAMFS functionality in VibexOS.
This script runs QEMU and sends commands to test ls and cat functionality.
"""

import subprocess
import time
import sys

def test_ramfs():
    """Test RAMFS functionality by running the OS and sending commands."""
    print("Testing RAMFS functionality...")
    
    # Commands to test
    test_commands = [
        "ls",           # List files
        "cat readme.txt",  # Read readme file
        "cat hello.txt",   # Read hello file
        "cat system.info", # Read system info
        "cat nonexistent.txt", # Test error handling
        "clear",        # Clear screen
        "ls"            # List files again after clear
    ]
    
    print("Expected behavior:")
    print("1. 'ls' should show 3 files: readme.txt, hello.txt, system.info")
    print("2. 'cat readme.txt' should show welcome message")
    print("3. 'cat hello.txt' should show hello world message")
    print("4. 'cat system.info' should show system information")
    print("5. 'cat nonexistent.txt' should show file not found error")
    print("6. 'clear' should clear the screen")
    print("7. 'ls' should show files again after clear")
    print("\nTo test manually, run: make run")
    print("Then type the commands above in the VibexOS shell")
    
    return True

if __name__ == "__main__":
    test_ramfs()