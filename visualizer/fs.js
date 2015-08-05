var fs = require('fs');

fs.readFile('dump.txt', 'utf8', function(err, data) {
    if (!err) {
        var r = data.split('\n');
        var points = {
            gx:[],
            gy:[],
            gz:[],
            wx:[],
            wy:[],
            wz:[]
        };
        for (var i in r) {
            var row = r[i].split('\t');
            if (row.length < 8) continue;
            var idx = 0;
            for (var t in points) {
                var value = parseFloat(row[2 + idx]);
                value = isNaN(value) ? 0 : value;
                points[t].push({x: parseInt(i), y: value});
                ++idx;
            }
        }
        console.log(points);
    }
});