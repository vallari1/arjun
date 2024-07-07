# #!/usr/bin/env python3
# import zmq
# import rclpy
# from rclpy.node import Node

# from std_msgs.msg import Float32MultiArray


# class Bridge(Node):

#     def __init__(self):
#         super().__init__('minimal_publisher')
#         self.publisher = self.create_publisher(Float32MultiArray, '/angles', 10)
#         timer_period = 1/10  # seconds
#         self.timer = self.create_timer(timer_period, self.timer_callback)
        
#         self.subscription = self.create_subscription(
#             Float32MultiArray,
#             '/wheel_vel',
#             self.listener_callback,
#             10)
        
#         c1 = zmq.Context()
#         self.sub = c1.socket(zmq.PAIR)

#         c2 = zmq.Context()
#         self.pub = c2.socket(zmq.PAIR)
#         self.pub.bind('tcp://127.0.0.1:1234')


#     def timer_callback(self):
#         self.sub.connect('tcp://127.0.0.1:1122')
#         data = self.sub.recv().decode("utf-8")
#         print(data)
#         imu = data.split(",")
#         wz = float(imu[0])
#         yaw = float(imu[1])
#         imu_msg = Float32MultiArray()
#         imu_msg.data = [wz, yaw]
#         self.publisher.publish(imu_msg)

#     def listener_callback(self, msg):
#         phi = msg.data
#         phi1 = phi[0]
#         phi2 = phi[1]
#         phi3 = phi[2]
#         phi4 = phi[3]
#         self.pub.send(f"{phi1},{phi2},{phi3},{phi4}".encode("utf-8"))

# def main(args=None):
#     rclpy.init(args=args)

#     minimal = Bridge()
#     rclpy.spin(minimal)

#     minimal.destroy_node()
#     rclpy.shutdown()


# if __name__ == '__main__':
#     main()

#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import Joy
#from std_msgs.msg import MultiArray
import serial

class Bridge(Node):
    def __init__(self):
        super().__init__('motor_bridge')
        self.latest_wheel_vel_data = []
        self.buttons = []

        timer_period = 1/10  
        self.timer = self.create_timer(timer_period, self.publish_data)
        
        self.subscription = self.create_subscription(
            Float32MultiArray,
            '/wheel_vel',
            self.wheel_vel_callback,
            10)
        
        self.button_subscription = self.create_subscription(
            Joy,
            'joy',
            self.button_callback,
            10)
        self.button_subscription
        try:
            self.ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
            self.get_logger().info('Connected to Arduino.')
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to connect to Arduino: {str(e)}')
            self.ser = None

    def wheel_vel_callback(self, msg):
        if self.ser is not None and len(msg.data) == 4:
            self.latest_wheel_vel_data = msg.data 
            motor_values = ",".join(map(str, msg.data))
            #self.get_logger().info(f"Motor values received: {motor_values}")
        else:
            self.get_logger().warn("Invalid message length or failed to connect to Arduino.")

    def button_callback(self, msg):
        self.buttons = [int(bool(state)) for state in msg.buttons]  # Convert bools to ints
        #self.get_logger().info(f"Button states received: {self.buttons}")

    def publish_data(self):
       
        if self.ser is not None:
            global buttons
            # Assuming the last 7 elements of msg.data contain button states
            # and the rest are motor values
            motor_values = ",".join(map(str, self.latest_wheel_vel_data))
            button_states = ",".join(map(str, self.buttons))
            
            # Concatenate and send the data
            data_to_send = f"{motor_values},{button_states}/n"
            self.ser.write(data_to_send.encode("utf-8"))
            print(f"Sending combined data: {data_to_send}")

def main(args=None):
    rclpy.init(args=args)
    motor_bridge = Bridge()
    try:
        rclpy.spin(motor_bridge)
    finally:
        motor_bridge.destroy_node()
        if hasattr(motor_bridge, 'ser') and motor_bridge is not None:
            motor_bridge.ser.close()
        rclpy.shutdown()

if __name__ == '__main__':
    main()

