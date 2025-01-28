import zmq
import streamlit as st

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

def send(message):
    try:
        socket.send(bytes(message, "utf-8"))
    except:
        print("Could not send")

def press_once():
    send("press-once")

def screenshot():
    send("screenshot")

st.title("WebApp")
st.button(label="Press once", on_click=press_once)
st.button(label="Screenshot", on_click=screenshot)
