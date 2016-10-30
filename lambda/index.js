var spawn = require('child_process').spawn;

exports.handler = (event, context, callback) => {
    console.log(JSON.stringify(event, 0, 2));
    // exec("./mkstr \"/w600/h100/s50/c888/bFFF/This is /cF00/test /c888/string.\"", (err, stdout, stderr) => {
    var buf=new Buffer(1024*1024);
    var len=0;
    var p=spawn("./mkstr");
    p.stdout.on('data',(d)=>{
        d.copy(buf,len);
        len+=d.length;
    });
    p.on('close',()=>{
        console.log("stdout " + len);
        callback(null, buf.toString('base64',0,len));
    });
};

