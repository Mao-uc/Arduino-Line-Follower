# Arduino Line Follower

**Course Project Task for the OUC Robotics Club**

This is one of the project tasks for the Robotics Club at OUC (Ocean University of China). **The task mandates the use of exactly three (or three-channel) infrared sensors.** The actual performance of the code may vary depending on the specific chassis structure. For our group's chassis design, the parameters have been finely tuned to near-optimal levels. A demonstration video has been uploaded to Bilibili.

---
### **Hardware Setup**
*   **Main Controller:** Arduino UNO R3 / R4
*   **Key Component:** As required by the task specifications, an **ADS1115 ADC converter** must be used.
    *   **Connection:** Connect the A0, A1, and A2 channels of the ADS1115 to the **D0 output pins** of the three IR sensors, respectively.

---
### **Practical Tip for 2WD Chassis**
For a 2WD chassis (consisting of two driven wheels and one caster ball/support wheel), it is highly recommended to **secure the swivel mechanism of the caster wheel** (e.g., using glue or a physical lock). This modification constrains its movement to a single direction, significantly reducing wobble and resulting in a much smoother and more stable line-following performance.

---
### **Video & Contact**
*   **Demonstration Video & Our Chassis:** You can see the working demonstration and our specific chassis structure in this video: **[https://b23.tv/8U5A5iw](https://b23.tv/8U5A5iw)**
*   **Questions?** Feel free to send a private message via Bilibili if you have any questions.
