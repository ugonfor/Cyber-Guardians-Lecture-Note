 // frida를 통해 mine.exe의 imagebase 구함
 var baseAddr = Module.findBaseAddress('mine.exe');
 console.log('mine.exe baseAddr: ' + baseAddr);

 var clickhandler = resolveAddress('0x100140C'); // Here we use the function address as seen in our disassembler

 // clickhandler 함수의 호출을 잡음
 Interceptor.attach(clickhandler, { // Intercept calls to our function

     // When function is called, print out its parameters
     onEnter: function (args) {
         console.log('');
         console.log('[+] Called clickhandler(' + clickhandler + ')');
         console.log('[+] x: ' + String(Number(args[0]) & 0xffff));
         console.log('[+] y: ' + String((Number(args[0]) & 0xffff0000) >> 16));
     },

     // When function is finished
     onLeave: function (retval) {
         console.log('[+] Returned from clickhandler: ' + retval);
     }
 });

 function dumpAddr(info, addr, size) {
     if (addr.isNull())
         return;

     console.log('Data dump ' + info + ' :');
     var buf = addr.readByteArray(size);

     // If you want color magic, set ansi to true
     console.log(hexdump(buf, { offset: 0, length: size, header: true, ansi: false }));
 }

 function resolveAddress(addr) {
     var idaBase = ptr('0x01000000'); // Enter the base address of jvm.dll as seen in your favorite disassembler (here IDA)
     var offset = ptr(addr).sub(idaBase); // Calculate offset in memory from base address in IDA database
     var result = baseAddr.add(offset); // Add current memory base address to offset of function to monitor
     console.log('[+] New addr=' + result); // Write location of function in memory to console
     return result;
 }