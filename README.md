# Milestone 1: Stranger Things Light Wall
For the first milestone, you will be building "Addressable" RGB LEDs which can be connected in series with one another and can have patterns generated from them. You will need to utilize two development boards for this: the first is the one to control the RGB LED, and the second is to be a master for the whole LED Chain.

## Background Theory
In the following scene from Netflix's "Stranger Things", Will Byers stuck in a parallel dimension can only communicate to his mother by causing Christmas Lights to light up. The mother then strings up a set of lights like a Ouija Board so they can communicate back and forth.

[![Stranger Things Light Wall](https://i.gyazo.com/0ec40d564af88528784912d678e72122.jpg)](https://www.youtube.com/watch?v=BlVN5Ukp7c8 "Stranger Things - Lights Wall Scene [Leg PT-BR]")

## Our Implementation

For starters, the entire network of devices will look similar to the image below.

![Network Layout](https://i.gyazo.com/9786b254eafdff55ef3b9259d1c7dbf9.png)

### RGB Node

Each of these RGB Nodes will be responsible for taking in a string of Hex values over the UART RX Line, using the 3 least significant bytes as their own RGB values. These RGB values then need to be converted and transformed into Duty Cycles to replicate the proper color. Since the incoming string may be at most 80 bytes, the remaining string after removing the 3 least significant bytes needs to be transmitted over the UART TX line to the next node.

![Node Layout](https://i.gyazo.com/b9eb35f557c10cead61346034936084b.png)

The RGB Node needs to be configured to initialize all timers and peripherals upon starting up and enter a Low Power Mode while waiting for a message over UART. When a message begins to be received over UART, the first 3 bytes should be stored into an RGB buffer which can then be used to set the duty cycles for the RGB LED. The rest of the message should be placed into a UART TX buffer and when the incoming message is complete, should then be sent out over the UART TX Line. Once the message has been transmitted, you should then set your RGB LED accordingly. The flow of this code can be seen below.

![Program Flow](https://i.gyazo.com/2cd40704327b558c01df0fb9c098e0e1.png)