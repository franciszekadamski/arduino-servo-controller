import zmq
import streamlit as st

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

def send(message):
    socket.send(bytes(message))

def press_once():
    send("press-once")

def screenshot():
    send("screenshot")

st.title("WebApp")
st.button(label="Press once", on_click=press_once)
st.button(label="Press once", on_click=screenshot)
