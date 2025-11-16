1.If run in succession too much the host can fail to bind, resulting in a crash. need to fix
2. Client unfortunately needs  >20kb ram
3. Sanitize data before its sent over sockets (e.g. convert ~/a to /home/$USER/a)
4. -Line 56 -- write so that buffer is written into filePath Variable