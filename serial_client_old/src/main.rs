use std::io;
use serial2::SerialPort;

fn serial_request(port: &SerialPort, message: &str) -> Result<Vec<f64>, io::Error> {
    let mut buffer = [0; 256];
    let serial_request_message = message.as_bytes();
    buffer[..serial_request_message.len()]
        .copy_from_slice(serial_request_message);

    port.write_all(serial_request_message)?;

    let read = port.read(&mut buffer)?;
    let message = match std::str::from_utf8(&buffer[..read]) {
        Ok(msg) => msg.trim(),
        Err(_) => return Err(io::Error::new(io::ErrorKind::InvalidData, "Invalid UTF-8 data")),
    };

    let values: Vec<f64> = message
        .trim()
        .split("-")
        .filter_map(|v| v.parse::<f64>().ok())
        .collect();

    if values.len() == 4 {
        let air_humidity_percent = values[0];
        let soil_humidity_percent = values[1];
        let air_temperature_c_degrees = values[2];
        let time_h = values.get(3).map(|&time_ms| time_ms / (1000.0 * 60.0 * 60.0)).expect("Could not convert ms to h");
        Ok(vec![
            air_humidity_percent,
            soil_humidity_percent,
            air_temperature_c_degrees,
            time_h
        ])
    } else {
        Err(io::Error::new(io::ErrorKind::InvalidData, format!("Invalid data received: {message}")))
    }
}


fn main() {
    let port = SerialPort::open("/dev/ttyACM0", 9600)
        .expect("Could not open serial connection");

    let message = "GET_SENSOR_DATA";
    if let Ok(values) = serial_request(&port, message) {
        println!("{values:?}");
    }
}
