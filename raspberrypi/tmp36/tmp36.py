import plotly
import json
import time
import readadc

username = 'username'
api_key = 'api_key'
stream_token = 'streamtoken'
stream_server = 'http://stream.plot.ly'

p = plotly.plotly(username, api_key)
p.ioff();

print p.plot([{'x': [], 'y': [], 'type': 'scatter',
            'stream': {'token': stream_token, 'maxpoints': 200}
          }], filename='Raspberry Pi Streaming Example Values', fileopt='overwrite')

# temperature sensor middle pin connected channel 0 of mcp3008
sensor_pin = 0
readadc.initialize()

i=0
s = plotly.stream(stream_token)

#the main sensor reading loop
while True:
		sensor_data = readadc.readadc(sensor_pin, readadc.PINS.SPICLK, readadc.PINS.SPIMOSI, readadc.PINS.SPIMISO, readadc.PINS.SPICS)
		millivolts = sensor_data * ( 3300.0 / 1024.0)
		# 10 mv per degree
		temp_C = ((millivolts - 100.0) / 10.0) - 40.0
		# convert celsius to fahrenheit
		temp_F = ( temp_C * 9.0 / 5.0 ) + 32
		# remove decimal point from millivolts
		millivolts = "%d" % millivolts
		# show only one decimal place for temprature and voltage readings
		temp_C = "%.1f" % temp_C
		temp_F = "%.1f" % temp_F
		s.write({'x': i, 'y': temp_C })
		i+=1
		# delay between stream posts
		time.sleep(0.25)
