use serial2::SerialPort;
use serial_client::serial_request;

fn main() {
    let port = SerialPort::open("/dev/ttyACM0", 9600)
        .expect("Could not open serial connection");

    let message = "GET_SENSOR_DATA";
    if let Ok(values) = serial_request(&port, message) {
        println!("{values:?}");
    }
}


