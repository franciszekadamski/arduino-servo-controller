use serial2::SerialPort;
use serial_client::SerialRequest;

fn main() {
    let port = SerialPort::open("/dev/ttyACM0", 9600)
        .expect("Could not open serial connection");

    let message = "GET_SENSOR_DATA";
    if let Ok(values) = port.serial_request(message) {
        println!("{values:?}");
    }
}


