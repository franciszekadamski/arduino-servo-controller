use serial2::SerialPort;

fn main() {
    let port = SerialPort::open("/dev/ttyACM0", 9600)
        .expect("Could not open serial connection");
    let mut buffer = [0; 256];

    let serial_request_message = b"GET_SENSOR_DATA";
    buffer[..serial_request_message.len()]
        .copy_from_slice(serial_request_message);

    match port.write(&buffer[..serial_request_message.len()]) {
        Ok(_) => println!("Request sent successfully"),
        Err(e) => eprintln!("Error sending request: {e:?}"),
    }

    let read = port.read(&mut buffer).unwrap();
    let message = std::str::from_utf8(&buffer[..read]).unwrap();

    let values: Vec<f64> = message
        .trim()
        .split("-")
        .filter_map(|v| v.parse::<f64>().ok())
        .collect();

    if values.len() == 4 {
        let air_humidity_percent = values[0];
        let soil_humidity_percent = values[1];
        let air_temperature_c_degrees = values[2];
        let time_h = values.get(3).map(|&time_ms| time_ms / (1000.0 * 60.0 * 60.0)).unwrap_or(0.0);

        println!("Air humidity: {air_humidity_percent}%");
        println!("Soil humidity: {soil_humidity_percent}%");
        println!("Air temperature: {air_temperature_c_degrees} C");
        println!("Up time: {time_h} h");
    } else {
        eprintln!("Invalid data received: {message:?}");
    }
}

