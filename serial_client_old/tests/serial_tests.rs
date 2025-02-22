use serial2::SerialPort;
use std::{fs::OpenOptions, io::Write, thread, time::Duration};

use serial_client::serial_request;


#[test]
fn test_serial_request_with_virtual_port() {
    let test_port_path = "/dev/pts/4";
    let write_port_path = "/dev/pts/5";

    let port = SerialPort::open(test_port_path, 9600)
        .expect("Failed to open test serial port");

    let mut write_port = OpenOptions::new()
        .write(true)
        .open(write_port_path)
        .expect("Failed to open paired test serial port");

    writeln!(write_port, "50.0-30.0-22.5-3600000").expect("Failed to write to test serial port");

    thread::sleep(Duration::from_millis(100));

    let result = serial_request(&port, "GET_SENSOR_DATA");
    assert!(result.is_ok());

    let values = result.unwrap();
    assert_eq!(values, vec![50.0, 30.0, 22.5, 1.0]);
}
