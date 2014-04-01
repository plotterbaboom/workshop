#Notes on Usage
Got a new Arduino? Have a sensor? Great! You'll be visualizing your sensor data in no time.
It's easy!

Thanks to Node.js and the Johnny-Five library, you can easily interface with your Arduino and get started working on things right away!


#Easy:

####Install node.js
[Download Here](http://nodejs.org/download)

####Create a folder for your sensor project and move into it
```
$ mkdir your-project-name
$ cd your-project-name
```
####Use the node package manager (npm) to install the required libraries.
```
$ npm install plotly
$ npm install johnny-five
```
####Download one of our example scripts into your project folder and type:
```
$ node example-script.js
```
Johnny-Five will automatically connect to your Arduino, and start streaming all the data to Plotly!
