'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

  let dbConfig = new Object();

  const mysql = require('mysql2/promise');

  let lib = ffi.Library('./libgpxparser', {

    'getGPXdescription' : [ 'string', ['string'] ],
    'getGPXroutes' : [ 'string', ['string'] ],
    'getGPXtracks' : [ 'string', ['string'] ],
    'getRouteData' : ['string', ['string', 'int'] ],
    'getTrackData' : ['string', ['string', 'int'] ],
    'renameRoute' : ['int', ['string', 'string', 'int'] ],
    'renameTrack' : ['int', ['string', 'string', 'int'] ],
    'createNewGPXFileFromGIU' : ['int', ['string', 'string'] ],
    'addRouteFromGUI' : ['int', ['string', 'string'] ],
    'addWaypointFromGUI' : ['int', ['string', 'string'] ],
    'getPathBetweenRoutes' : ['string', ['string', 'float', 'float', 'float', 'float'] ],
    'getPathBetweenTracks' : ['string', ['string', 'float', 'float', 'float', 'float'] ],
    'getRouteWpts' : ['string', ['string', 'int'] ],


  });




app.get('/getGPXFiles', function(req , res){


  var myFileNames = new Array();
  var gpxArray = new Array();


  fs.readdir('./uploads/', (err, files) =>
  {
    if (err) {
      console.error(err);
    }
    files.forEach(file => {

      //add file names in array


      //add gpx JSONS to Array
      if(String(file).endsWith(".gpx") == 1){



        let gpx = lib.getGPXdescription('./uploads/' + file);

        if(gpx != ""){

          myFileNames.push(file);
          let gpxParsed = JSON.parse(gpx);
          gpxArray.push(gpxParsed);
        }


        //console.log(gpxParsed);
      }



    });



    res.send({
    fileNames: myFileNames,
    gpxInfo: gpxArray,
     });
  });

});

app.get('/getGPXElements', function(req , res){

  let file = req.query.fileName;
  //console.log(str);
  let tmpStr;

  tmpStr = lib.getGPXroutes('./uploads/' + file);
  tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
  let routesArr = JSON.parse(tmpStr);

  tmpStr = lib.getGPXtracks('./uploads/' + file);
  tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
  let tracksArr = JSON.parse(tmpStr);

  var tracksandroutes = {
    myRoutes: routesArr,
    myTracks: tracksArr
  };

  //console.log(tracksandroutes);

  res.send({
    gpxElements: tracksandroutes,
  });
});

app.get('/getGPXData', function(req , res){

  let type = req.query.type;
  let fileName = req.query.fileName;
  let index = req.query.index;
  //console.log(str);

  let dataArr;
  let tmpStr;
  if(type == 1){

    tmpStr = lib.getRouteData(('./uploads/' + fileName), index);
    tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
    dataArr = JSON.parse(tmpStr);
  }

  if(type == 2){
    tmpStr = lib.getTrackData(('./uploads/' + fileName), index);
    tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
    dataArr = JSON.parse(tmpStr);
  }

  //console.log(dataArr);

  res.send({
    gpxData: dataArr,
  });
});

async function renameRouteOnDB(file, index, newName){

  let connection;

  try{
    connection = await mysql.createConnection(dbConfig);

    //get id num from gpx file
    const [rows, fields] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name = '" + String(file) + "';" );
    let gpx_id_num = rows[0].gpx_id;

    //get route from selected file
    const [rows1, fields1] = await connection.execute("SELECT route_id FROM ROUTE WHERE gpx_id = " + gpx_id_num + ";" );
    let route_id_num = rows1[index].route_id;

    await connection.execute("UPDATE ROUTE SET route_name='"+ newName + "' WHERE route_id = " + route_id_num + ";");


  }catch(e){
    console.log("Query error: " + e);
  }finally{
    if (connection && connection.end) connection.end();
  }
}

app.get('/rename', function(req , res){
  let type = req.query.type;
  let fileName = req.query.fileName;
  let index = req.query.index;
  let newName = req.query.newName;
  let returnVal;



  if(type == 'route'){
    returnVal = lib.renameRoute(('./uploads/' + fileName), newName, index);
    renameRouteOnDB(fileName, index, newName);
  }

  if(type == 'track'){
    returnVal = lib.renameTrack(('./uploads/' + fileName), newName, index);
  }

  res.send({
    success: returnVal
  });
});

async function createGPXOnDB(file, version, creator){

  let connection;

  try{
    connection = await mysql.createConnection(dbConfig);

    let query = "INSERT INTO FILE(file_name, ver, creator) VALUES ('"+String(file)+ "'," + version+ ",'" + creator +  "' );  "

    await connection.execute(query);


  }catch(e){
    console.log("Query error: " + e);
  }finally{
    if (connection && connection.end) connection.end();
  }
}

app.get('/createGPX', function(req , res){

  let fileName = req.query.fileName;
  let version = req.query.version;
  let creator = req.query.creator;
  let success = 1;
  let writeSucess;


    fs.readdir('./uploads/', (err, files) =>
    {
      if (err) {
        console.error(err);
      }
      files.forEach(file => {
        //add file names in array
          if(file == fileName){
            success = 0;
          }
      });

  let gpx = new Object();
  gpx.version = version;
  gpx.creator = creator;
  let gpxStr = JSON.stringify(gpx);

  writeSucess = lib.createNewGPXFileFromGIU(gpxStr,('./uploads/' + fileName));

  if(success == 1 && writeSucess == 1){
    createGPXOnDB(fileName, version, creator);
  }


  res.send({
    success: success,
    writeSucess: writeSucess,
  });
  });
});


async function addRouteOnDB(file, route, waypoints){

  let connection;

  try{
    connection = await mysql.createConnection(dbConfig);

    //get id num from gpx file
    const [rows, fields] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name = '" + String(file) + "';" );
    let gpx_id_num = rows[0].gpx_id;


    let routeQuery = "INSERT INTO ROUTE(route_name, route_len, gpx_id) VALUES ('"+route.name+ "'," + route.len+ "," + gpx_id_num +  " );  "
    await connection.execute(routeQuery);


    //get route from selected file
    const [rows1, fields1] = await connection.execute("SELECT route_id FROM ROUTE WHERE gpx_id = " + gpx_id_num + ";" );
    let route_id_num = rows1[rows1.length-1].route_id;

    //get point index
    const [rows2, fields2] = await connection.execute("SELECT point_id FROM POINT WHERE route_id = " + route_id_num + ";" );
    let pointIndex = rows2.length-1;
    if(rows2.length == 0){
      pointIndex = 0;
    }

    let wptQuery;
    for(let wpt of waypoints){

      wpt=JSON.parse(wpt);

      wptQuery = "INSERT INTO POINT(point_index, latitude, longitude, route_id) VALUES ("+pointIndex+ "," + wpt.lat+ "," + wpt.lon + ","+route_id_num+  " );  "
      await connection.execute(wptQuery);
      pointIndex++;
    }


  }catch(e){
    console.log("Query error: " + e);
  }finally{
    if (connection && connection.end) connection.end();
  }
}

app.get('/addRoute', function(req , res){

  let fileName = req.query.fileName;
  let route = req.query.route;
  let waypoints = req.query.waypointArr;
  let length = req.query.length;

  /*console.log(fileName);
  console.log(route);
  console.log(waypoints);*/


  let returnVal = lib.addRouteFromGUI(('./uploads/' + fileName), route);

  let wptVal;
  for(let i in waypoints){
    wptVal = lib.addWaypointFromGUI(('./uploads/' + fileName), waypoints[i]);
  }

  let tmpStr;

  tmpStr = lib.getGPXroutes('./uploads/' + fileName);
  tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
  let routesArr = JSON.parse(tmpStr);

  let newRoute = routesArr[routesArr.length-1];

  addRouteOnDB(fileName, newRoute, waypoints);

  res.send({
    success: returnVal,
  });
});

app.get('/findPath', function(req , res){

  let startLat = req.query.startLat;
  let startLong = req.query.startLong;
  let endLat = req.query.endLat;
  let endLong = req.query.endLong;
  let files = req.query.files;

  /*console.log(fileName);
  console.log(route);
  console.log(waypoints);*/
  let components = [];
  let components2 = [];
  let routes;
  let parsedRoutes;
  let tracks;
  let parsedTracks;

  for(let i in files){
    routes = lib.getPathBetweenRoutes(('./uploads/' + files[i]), startLat, startLong, endLat, endLong);
    parsedRoutes = JSON.parse(routes);

    if(parsedRoutes.length > 0){
      components = components.concat(parsedRoutes);
    }

    tracks = lib.getPathBetweenTracks(('./uploads/' + files[i]), startLat, startLong, endLat, endLong);
    parsedTracks = JSON.parse(tracks);

    if(parsedTracks.length > 0){
      components2 = components2.concat(parsedTracks);
    }

  }

  //console.log(components);

  res.send({
    routes: components,
    tracks: components2,
  });
});

app.get('/dbLogin', async function(req, res, next){

    dbConfig = req.query.conf;

    let success = 0;
    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);
      await connection.execute(
        "CREATE TABLE IF NOT EXISTS FILE ( gpx_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, ver DECIMAL(2,1) NOT NULL, creator VARCHAR(256) NOT NULL );"
      );

      await connection.execute(
        "CREATE TABLE IF NOT EXISTS ROUTE ( route_id INT AUTO_INCREMENT PRIMARY KEY, route_name VARCHAR(256), route_len FLOAT(15,7) NOT NULL, gpx_id INT NOT NULL, FOREIGN KEY(gpx_id) REFERENCES FILE(gpx_id) ON DELETE CASCADE );"
      );

      await connection.execute(
        "CREATE TABLE IF NOT EXISTS POINT ( point_id INT AUTO_INCREMENT PRIMARY KEY, point_index INT NOT NULL, latitude DECIMAL(11,7) NOT NULL, longitude DECIMAL(11,7) NOT NULL, point_name VARCHAR(256), route_id INT NOT NULL, FOREIGN KEY(route_id) REFERENCES ROUTE(route_id) ON DELETE CASCADE );"
      );

      success = 1;
      res.send({
        success: success,
      });
    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }


});

app.get('/dbStoreFiles', async function(req, res, next){


    let files = req.query.files;
    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);



      let gpx;
      let gpxParsed
      let query;

      for(let file of files){

        gpx = lib.getGPXdescription('./uploads/' + file);
        gpxParsed = JSON.parse(gpx);
        //console.log("SELECT file_name FROM FILE WHERE file_name = " + String(file) + ";")
        const [rows, fields] = await connection.execute("SELECT file_name FROM FILE WHERE file_name = '" + String(file) + "';" );

        //If not alredy in table
        if(rows.length == 0){

          query = "INSERT INTO FILE(file_name, ver, creator) VALUES ('"+String(file)+ "'," + gpxParsed.version+ ",'" + gpxParsed.creator +  "' );  "
        //  console.log(query);
          await connection.execute(query);

          const [rows1, fields1] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name = '" + String(file) + "';" );

          let id_num = rows1[0].gpx_id;

          let tmpStr;

          tmpStr = lib.getGPXroutes('./uploads/' + file);
          tmpStr = tmpStr.replace(/(\r\n|\n|\r)/gm, "");
          let routesArr = JSON.parse(tmpStr);

          let routeQuery;
          let index = 0;

          for(let route of routesArr){

            routeQuery = "INSERT INTO ROUTE(route_name, route_len, gpx_id) VALUES ('"+route.name+ "'," + route.len+ "," + id_num +  " );  "
            await connection.execute(routeQuery);

            let wptStr = lib.getRouteWpts('./uploads/' + file, index);
            wptStr = wptStr.replace(/(\r\n|\n|\r)/gm, "");
            let wptsArr = JSON.parse(wptStr);

            const [rows2, fields2] = await connection.execute("SELECT route_id FROM ROUTE WHERE route_name = '" + route.name + "';" );

            let rte_id_num = rows2[0].route_id;

            let wptIndex = 0;
            let wptQuery;

            for(let wpt of wptsArr){

              wptQuery = "INSERT INTO POINT(point_index, latitude, longitude, point_name, route_id) VALUES ("+wptIndex+ "," + wpt.lat+ "," + wpt.lon + ",'"+wpt.name+"',"+rte_id_num+  " );  "
              await connection.execute(wptQuery);

              wptIndex++;
            }

            //console.log(wptsArr);
            index = index + 1;
          }

        }




      }


          await connection.execute("UPDATE ROUTE SET route_name=NULL WHERE route_name = '';");
          await connection.execute("UPDATE POINT SET point_name=NULL WHERE point_name = '';");

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }


});

app.get('/dbClear', async function(req, res, next){


    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      let clearQuery = "DELETE FROM FILE;"
      await connection.execute(clearQuery);


    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }


});

app.get('/dbStatus', async function(req, res, next){


    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows, fields] = await connection.execute("SELECT COUNT(*) AS num FROM FILE;" );
      const [rows1, fields1] = await connection.execute("SELECT COUNT(*) AS num FROM ROUTE;" );
      const [rows2, fields2] = await connection.execute("SELECT COUNT(*) AS num FROM POINT;" );

      let fileCount = rows[0].num;
      let routeCount = rows1[0].num;
      let pointCount = rows2[0].num;

      res.send({
        file: fileCount,
        route: routeCount,
        point: pointCount,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }



});

app.get('/query1Name', async function(req, res, next){


    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE ORDER BY route_name;" );

      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query1Len', async function(req, res, next){


    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE ORDER BY route_len;" );

      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query2Name', async function(req, res, next){

    let fileName = req.query.fileName;


    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      const [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_name;" );

      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query2Len', async function(req, res, next){

    let fileName = req.query.fileName;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      const [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len;" );

      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query3', async function(req, res, next){

    let fileName = req.query.fileName;
    let index = req.query.index;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      const [rows, fields] = await connection.execute("SELECT route_id, route_len FROM ROUTE WHERE gpx_id ="+fileNum+";" );
      let rte_id = rows[index].route_id;

      const [rows3, fields3] = await connection.execute("SELECT * FROM POINT WHERE route_id ="+rte_id+" ORDER BY point_index;" );

      res.send({
        points: rows3,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query4', async function(req, res, next){

    let fileName = req.query.fileName;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      const [rows3, fields3] = await connection.execute("SELECT point_index, latitude, longitude, route_name, route_len FROM POINT, ROUTE, FILE WHERE FILE.gpx_id = ROUTE.gpx_id AND ROUTE.route_id = POINT.route_id AND FILE.gpx_id ="+fileNum+" ORDER BY route_len;" );


      res.send({
        points: rows3,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query4Name', async function(req, res, next){

    let fileName = req.query.fileName;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      const [rows3, fields3] = await connection.execute("SELECT point_index, latitude, longitude, route_name, route_len FROM POINT, ROUTE, FILE WHERE FILE.gpx_id = ROUTE.gpx_id AND ROUTE.route_id = POINT.route_id AND FILE.gpx_id ="+fileNum+" ORDER BY route_name;" );


      res.send({
        points: rows3,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query5Len', async function(req, res, next){

    let fileName = req.query.fileName;
    let ls = req.query.longorshort;
    let n = req.query.n;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      //let [rows, fields];
      let [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len ASC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_len;" );
      if(String(ls) == "shortest"){
        [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len ASC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_len;" );
      }else{
        [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len DESC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_len;" );
      }

      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});

app.get('/query5Name', async function(req, res, next){

    let fileName = req.query.fileName;
    let ls = req.query.longorshort;
    let n = req.query.n;

    let connection;

    try{
      connection = await mysql.createConnection(dbConfig);

      const [rows2, fields2] = await connection.execute("SELECT gpx_id FROM FILE WHERE file_name ='"+fileName+"';" );
      let fileNum = rows2[0].gpx_id;

      let [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len ASC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_name;" );
      if(String(ls) == "shortest"){
        [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len ASC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_name;" );
      }else{
        [rows, fields] = await connection.execute("SELECT route_name, route_len FROM ROUTE AS F INNER JOIN (SELECT route_id FROM ROUTE WHERE gpx_id ="+fileNum+" ORDER BY route_len DESC LIMIT "+n+") AS F2 ON F.route_id = F2.route_id ORDER BY route_name;" );
      }
      res.send({
        routes: rows,
      });

    }catch(e){
      console.log("Query error: " + e);
    }finally{
      if (connection && connection.end) connection.end();
    }

});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
