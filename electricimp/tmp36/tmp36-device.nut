// initialize pins
local tmp36 = hardware.pin1;
tmp36.configure(ANALOG_IN);

// function returns temperature from tmp36
function getTemperature() {
    local supplyVoltage = hardware.voltage();
    local voltage = supplyVoltage * tmp36.read() / 65535.0;
    local c = (voltage - 0.5) * 100 ;
    local celsius = format("%.01f", c);
    return(celsius);
}

// function to send sensor data to be plotted
function sendDataToAgent() {
    local supplyVoltage = hardware.voltage();
    local sensor_voltage = getTemperature();

    local sensordata = {
        sensor_reading = sensor_voltage,
        time_stamp = getTime()
    }
    agent.send("new_readings", sensordata);
    // How often to make http request (seconds)
    imp.wakeup(1800, sendDataToAgent);
}


// returns time string, -14400 is for -4 GMT (Montreal)
// use 3600 and multiply by the hours +/- GMT.
// e.g for +5 GMT local date = date(time()+18000, "u");
function getTime() {
    local date = date(time()-14400, "u");
    local sec = stringTime(date["sec"]);
    local min = stringTime(date["min"]);
    local hour = stringTime(date["hour"]);
    local day = stringTime(date["day"]);
    local month = date["month"];
    local year = date["year"];
    return year+"-"+month+"-"+day+" "+hour+":"+min+":"+sec;

}

// function to fix time string
function stringTime(num) {
    if (num < 10)
        return "0"+num;
    else
        return ""+num;
}

// Initialize Loop
sendDataToAgent();
