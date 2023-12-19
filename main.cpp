#include "mbed.h"
#include "wifi_helper.h"
#include "mbed-trace/mbed_trace.h"

// Sensors drivers present in the BSP library
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#include <string>

// Buzzer frequency
#include "pitches.h"

DigitalOut led(LED1);






// -----------------------------------     Final project     -----------------------------------

// Input devices : joystick, button, and buzzer
AnalogIn joystick_1_x(A0);
AnalogIn joystick_1_y(A1);
AnalogIn joystick_2_x(A2);
AnalogIn joystick_2_y(A3);

DigitalIn button_1(D8);
DigitalIn button_2(D7);
DigitalIn button_pause(D4);

PwmOut buzzer(D9);

// Joystick range
/*
    x0      x1      x2      x3
y0__|_______|_______|_______|
    |       |       |       |
    |   1   |   2   |   3   |
y1__|_______|_______|_______|
    |       |       |       |
    |   4   |   5   |   6   |
y2__|_______|_______|_______|
    |       |       |       |
    |   7   |   8   |   9   |
y3__|_______|_______|_______|
*/

float x_boundary_1[4] = {0, 0.5, 0.88, 1};   // center : 0.774 - 0.779
float y_boundary_1[4] = {0, 0.7, 0.83, 1};   // center : 0.737 - 0.741
float x_boundary_2[4] = {0, 0.5, 0.83, 1};   // center : 0.755 - 0.759
float y_boundary_2[4] = {0, 0.7, 0.8, 1};   // center : 0.749 - 0.752

// Buzzer
int note_division = 16;
int total_note = 194;  // 26 + 32 + 28 + 30 + 33 + 29 + 16 = 194
int note[] = {
                // 1 - 4 : 26 notes
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
                NOTE_A3, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_B3,
                NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_G3, NOTE_G3,

                // 5 - 9 : 32 notes
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
                NOTE_A3, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_G4,
                NOTE_A4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4,
                NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,

                // 10 - 14 : 28 notes
                NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
                NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
                NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G4,
                NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
                NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,

                // 15 - 19 : 30 notes
                NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
                NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
                NOTE_A3, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_B3,

                // 20 - 24 : 33 notes
                NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_G3, NOTE_G3,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3,
                NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
                NOTE_A3, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_G4,
                NOTE_A4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4,

                // 25 - 29 : 29 notes
                NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
                NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
                NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
                NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G4,
                NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,

                // 30 - 32 : 16 notes
                NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
                NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
                NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4
             };

float duration[] = {
                        // 1 - 4 : 26 notes
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2,
                        8, 8, 8, 8, 2.666, 16, 16,

                        // 5 - 9 : 32 notes
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 8, 8, 4,
                        8, 8, 8, 8, 2,
                        8, 8, 4, 8, 8, 4,

                        // 10 - 14 : 28 notes
                        8, 8, 5.333, 16, 2,
                        8, 8, 4, 8, 8, 4,
                        8, 8, 8, 8, 4, 4,
                        8, 8, 4, 8, 8, 4,
                        8, 8, 5.333, 16, 2,

                        // 15 - 19 : 30 notes
                        8, 8, 4, 8, 8, 4,
                        8, 8, 8, 8, 2,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2,

                        // 20 - 24 : 33 notes
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 2.666, 16, 16,
                        8, 8, 8, 8, 8, 8, 4,
                        8, 8, 8, 8, 2,

                        // 25 - 29 : 29 notes
                        8, 8, 4, 8, 8, 4,
                        8, 8, 5.333, 16, 2,
                        8, 8, 4, 8, 8, 4,
                        8, 8, 8, 8, 4, 4,
                        8, 8, 4, 8, 8, 4,

                        // 30 - 32 : 16 notes
                        8, 8, 5.333, 16, 2,
                        8, 8, 4, 8, 8, 4,
                        8, 8, 8, 8, 2
                   };

// -----------------------------------     Final project     -----------------------------------






#if MBED_CONF_APP_USE_TLS_SOCKET
#include "root_ca_cert.h"

#ifndef DEVICE_TRNG
#error "mbed-os-example-tls-socket requires a device which supports TRNG"
#endif
#endif // MBED_CONF_APP_USE_TLS_SOCKET

class SocketDemo {
    static constexpr size_t MAX_NUMBER_OF_ACCESS_POINTS = 10;
    static constexpr size_t MAX_MESSAGE_RECEIVED_LENGTH = 100;

#if MBED_CONF_APP_USE_TLS_SOCKET
    static constexpr size_t REMOTE_PORT = 443; // tls port
#else
    static constexpr size_t REMOTE_PORT = 9999; // standard HTTP port
#endif // MBED_CONF_APP_USE_TLS_SOCKET

public:
    SocketDemo() : _net(NetworkInterface::get_default_instance())
    {
    }

    ~SocketDemo()
    {
        if (_net) {
            _net->disconnect();
        }
    }

    


    // -----------------------------------     Final project     -----------------------------------

    void detect_stm_sensor_value()
    {
        // Sensor init
        printf("Start sensor init\n");

        BSP_TSENSOR_Init();
        BSP_HSENSOR_Init();
        BSP_PSENSOR_Init();

        BSP_MAGNETO_Init();
        BSP_GYRO_Init();
        BSP_ACCELERO_Init();

        // Sensor detection
        float sensor_value = 0;
        int16_t pDataXYZ[3] = {0};
        float pGyroDataXYZ[3] = {0};

        while(1)
        {
            // Loop begin
            printf("\nNew loop, LED1 should blink during sensor read\n\n");
            led = 1;

            // Record sensor data
            string data = "";

            // Collecting sensor data
            sensor_value = BSP_TSENSOR_ReadTemp();       
            printf("TEMPERATURE = %.2f degC\n", sensor_value); 
            data = data + to_string(sensor_value) + " ";

            sensor_value = BSP_HSENSOR_ReadHumidity();
            printf("HUMIDITY    = %.2f %%\n", sensor_value);
            data = data + to_string(sensor_value) + " ";
            
            sensor_value = BSP_PSENSOR_ReadPressure();
            printf("PRESSURE is = %.2f mBar\n", sensor_value);
            data = data + to_string(sensor_value) + " ";
            
            BSP_MAGNETO_GetXYZ(pDataXYZ);           
            printf("MAGNETO_X = %d\n", pDataXYZ[0]);
            printf("MAGNETO_Y = %d\n", pDataXYZ[1]);
            printf("MAGNETO_Z = %d\n", pDataXYZ[2]);
            data = data + to_string(pDataXYZ[0]) + " " + to_string(pDataXYZ[1]) + " " + to_string(pDataXYZ[2]) + " ";

            BSP_GYRO_GetXYZ(pGyroDataXYZ);           
            printf("GYRO_X = %.2f\n", pGyroDataXYZ[0]);
            printf("GYRO_Y = %.2f\n", pGyroDataXYZ[1]);
            printf("GYRO_Z = %.2f\n", pGyroDataXYZ[2]);
            data = data + to_string(pGyroDataXYZ[0]) + " " + to_string(pGyroDataXYZ[1]) + " " + to_string(pGyroDataXYZ[2]) + " ";

            BSP_ACCELERO_AccGetXYZ(pDataXYZ);           
            printf("ACCELERO_X = %d\n", pDataXYZ[0]);
            printf("ACCELERO_Y = %d\n", pDataXYZ[1]);
            printf("ACCELERO_Z = %d\n", pDataXYZ[2]);
            data = data + to_string(pDataXYZ[0]) + " " + to_string(pDataXYZ[1]) + " " + to_string(pDataXYZ[2]) + " ";
            
            // Convert string into char array
            const char* data_to_sent = data.c_str();

            // Send sensor data
            if(!send_http_request(data_to_sent)) {return;}

            // Wait for the next loop
            led = 0;        
            ThisThread::sleep_for(1s);

        }
    }

    int detect_area_1(float x, float y)
    {
        int area = 5;

        // Error correction : joystick sensor
        if(0.72 < y && y < 0.73) {y = 0.99;}
        
        // Detect area
        if(x < x_boundary_1[0])
        {
            area = 0;
        }
        else if(x <= x_boundary_1[1])
        {
            if(y < y_boundary_1[0]) {area = 0;}
            else if(y <= y_boundary_1[1]) {area = 1;}
            else if(y <= y_boundary_1[2]) {area = 4;}
            else if(y <= y_boundary_1[3]) {area = 7;}
            else {area = 0;}
        }
        else if(x <= x_boundary_1[2])
        {
            if(y < y_boundary_1[0]) {area = 0;}
            else if(y <= y_boundary_1[1]) {area = 2;}
            else if(y <= y_boundary_1[2]) {area = 5;}
            else if(y <= y_boundary_1[3]) {area = 8;}
            else {area = 0;}
        }
        else if(x <= x_boundary_1[3])
        {
            if(y < y_boundary_1[0]) {area = 0;}
            else if(y <= y_boundary_1[1]) {area = 3;}
            else if(y <= y_boundary_1[2]) {area = 6;}
            else if(y <= y_boundary_1[3]) {area = 9;}
            else {area = 0;}
        }
        else
        {
            area = 0;
        }

        return area;
    }

    int detect_area_2(float x, float y)
    {
        int area = 5;

        // Error correction : joystick sensor
        if(0.725 < x && x < 0.732) {x = 0.99;}
        if(0.72 < y && y < 0.73) {y = 0.99;}       
        
        // Detect area
        if(x < x_boundary_2[0])
        {
            area = 0;
        }
        else if(x <= x_boundary_2[1])
        {
            if(y < y_boundary_2[0]) {area = 0;}
            else if(y <= y_boundary_2[1]) {area = 1;}
            else if(y <= y_boundary_2[2]) {area = 4;}
            else if(y <= y_boundary_2[3]) {area = 7;}
            else {area = 0;}
        }
        else if(x <= x_boundary_2[2])
        {
            if(y < y_boundary_2[0]) {area = 0;}
            else if(y <= y_boundary_2[1]) {area = 2;}
            else if(y <= y_boundary_2[2]) {area = 5;}
            else if(y <= y_boundary_2[3]) {area = 8;}
            else {area = 0;}
        }
        else if(x <= x_boundary_2[3])
        {
            if(y < y_boundary_2[0]) {area = 0;}
            else if(y <= y_boundary_2[1]) {area = 3;}
            else if(y <= y_boundary_2[2]) {area = 6;}
            else if(y <= y_boundary_2[3]) {area = 9;}
            else {area = 0;}
        }
        else
        {
            area = 0;
        }

        return area;
    }

    void play_game()
    {
        
        bool if_play_music = false;
        int note_count = 0;
        int note_count_branch = 0;

        printf("\nGame start!\n");

        while(1)
        {
            
            // Joystick 1
            float joystick_1_x_value = joystick_1_x;
            float joystick_1_y_value = joystick_1_y;
            int joystick_1_area = detect_area_1(joystick_1_x_value, joystick_1_y_value);
            //printf("joystick 1 : x = %f, y = %f\n", joystick_1_x_value, joystick_1_y_value);

            // Joystick 2
            float joystick_2_x_value = joystick_2_x;
            float joystick_2_y_value = joystick_2_y;
            int joystick_2_area = detect_area_2(joystick_2_x_value, joystick_2_y_value);
            //printf("joystick 2 : x = %f, y = %f\n", joystick_2_x_value, joystick_2_y_value);

            // Button 1
            bool button_1_pressed = false;
            if(button_1) {button_1_pressed = true;}           
            //printf("botton 1 : pressed = %f\n", float(button_1_pressed));

            // Button 2
            bool button_2_pressed = false;
            if(button_2) {button_2_pressed = true;}          
            //printf("botton 2 : pressed = %f\n", float(button_2_pressed));

            // Button pause
            bool button_pause_pressed = false;
            if(button_pause)
            {
                button_pause_pressed = true;
                if_play_music = !if_play_music;
                ThisThread::sleep_for(200ms);
            }          
            //printf("botton pause : pressed = %f\n", float(button_pause_pressed));
            
            // Buzzer
            if(if_play_music)
            {
                buzzer.period(1.0 / note[note_count]);
                buzzer.write(0.7);
                //wait_us(480000.0 / duration[note_count]);
                wait_us(120000.0 / duration[note_count]);

                note_count_branch += 1;

                if(note_count_branch >= note_division)
                {
                    buzzer.write(0);
                    wait_us(10000);
                    
                    note_count_branch = 0;
                    note_count += 1;
                    if(note_count >= total_note)
                    {
                        note_count = 0;
                    }
                }
            }
            else
            {
                buzzer.write(0);
            }
            
    

            // Data
            string data = "";
            data = data + to_string(joystick_1_area) + to_string(button_1_pressed) + to_string(joystick_2_area) + to_string(button_2_pressed)+ to_string(button_pause_pressed);

            // Send data to python
            const char* data_to_sent = data.c_str();
            //printf("data = %s\n", data_to_sent);
            send_http_request(data_to_sent);

            // Wait for the next loop
            ThisThread::sleep_for(10ms);
            //ThisThread::sleep_for(100ms);

        }
        






        

    }



    // -----------------------------------     Final project     -----------------------------------






    void run()
    {
        if (!_net) {
            printf("Error! No network interface found.\r\n");
            return;
        }

        /* if we're using a wifi interface run a quick scan */
        if (_net->wifiInterface()) {
            /* the scan is not required to connect and only serves to show visible access points */
            wifi_scan();

            /* in this example we use credentials configured at compile time which are used by
             * NetworkInterface::connect() but it's possible to do this at runtime by using the
             * WiFiInterface::connect() which takes these parameters as arguments */
        }

        /* connect will perform the action appropriate to the interface type to connect to the network */

        printf("Connecting to the network...\r\n");

        nsapi_size_or_error_t result = _net->connect();
        if (result != 0) {
            printf("Error! _net->connect() returned: %d\r\n", result);
            return;
        }

        print_network_info();

        /* opening the socket only allocates resources */
        result = _socket.open(_net);
        if (result != 0) {
            printf("Error! _socket.open() returned: %d\r\n", result);
            return;
        }

#if MBED_CONF_APP_USE_TLS_SOCKET
        result = _socket.set_root_ca_cert(root_ca_cert);
        if (result != NSAPI_ERROR_OK) {
            printf("Error: _socket.set_root_ca_cert() returned %d\n", result);
            return;
        }
        _socket.set_hostname(MBED_CONF_APP_HOSTNAME);
#endif // MBED_CONF_APP_USE_TLS_SOCKET

        /* now we have to find where to connect */

        SocketAddress address;

        if (!resolve_hostname(address)) {
            return;
        }

        address.set_port(REMOTE_PORT);

        /* we are connected to the network but since we're using a connection oriented
         * protocol we still need to open a connection on the socket */

        printf("Opening connection to remote port %d\r\n", REMOTE_PORT);
        
        result = _socket.connect(address);
        if (result != 0) {
            printf("Error! _socket.connect() returned: %d\r\n", result);
            return;
        }

        play_game();
        
    }




private:
    bool resolve_hostname(SocketAddress &address)
    {
        const char hostname[] = MBED_CONF_APP_HOSTNAME;

        /* get the host address */
        printf("\nResolve hostname %s\r\n", hostname);
        nsapi_size_or_error_t result = _net->gethostbyname(hostname, &address);
        if (result != 0) {
            printf("Error! gethostbyname(%s) returned: %d\r\n", hostname, result);
            return false;
        }

        printf("%s address is %s\r\n", hostname, (address.get_ip_address() ? address.get_ip_address() : "None") );

        return true;
    }

    bool send_http_request(const char *buffer)
    {       
        //char buffer[120] = "12345";
        //sprintf(buffer, "%.2f", message);

        nsapi_size_t bytes_to_send = strlen(buffer);
        nsapi_size_or_error_t bytes_sent = 0;

        //printf("Sending message: %s\n", buffer);

        _socket.send(buffer, bytes_to_send);

        return true;
    }

    bool receive_http_response()
    {
        char buffer[MAX_MESSAGE_RECEIVED_LENGTH];
        int remaining_bytes = MAX_MESSAGE_RECEIVED_LENGTH;
        int received_bytes = 0;

        /* loop until there is nothing received or we've ran out of buffer space */
        nsapi_size_or_error_t result = remaining_bytes;
        while (result > 0 && remaining_bytes > 0) {
            result = _socket.recv(buffer + received_bytes, remaining_bytes);
            if (result < 0) {
                printf("Error! _socket.recv() returned: %d\r\n", result);
                return false;
            }

            received_bytes += result;
            remaining_bytes -= result;
        }

        /* the message is likely larger but we only want the HTTP response code */

        printf("received %d bytes:\r\n%.*s\r\n\r\n", received_bytes, strstr(buffer, "\n") - buffer, buffer);

        return true;
    }

    void wifi_scan()
    {
        WiFiInterface *wifi = _net->wifiInterface();

        WiFiAccessPoint ap[MAX_NUMBER_OF_ACCESS_POINTS];

        /* scan call returns number of access points found */
        int result = wifi->scan(ap, MAX_NUMBER_OF_ACCESS_POINTS);

        if (result <= 0) {
            printf("WiFiInterface::scan() failed with return value: %d\r\n", result);
            return;
        }

        printf("%d networks available:\r\n", result);

        for (int i = 0; i < result; i++) {
            printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n",
                   ap[i].get_ssid(), get_security_string(ap[i].get_security()),
                   ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
                   ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
                   ap[i].get_rssi(), ap[i].get_channel());
        }
        printf("\r\n");
    }

    void print_network_info()
    {
        /* print the network info */
        SocketAddress a;
        _net->get_ip_address(&a);
        printf("IP address: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_netmask(&a);
        printf("Netmask: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_gateway(&a);
        printf("Gateway: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    }

private:
    NetworkInterface *_net;

#if MBED_CONF_APP_USE_TLS_SOCKET
    TLSSocket _socket;
#else
    TCPSocket _socket;
#endif // MBED_CONF_APP_USE_TLS_SOCKET
};

int main() {
    printf("\r\nStarting socket demo\r\n\r\n");

#ifdef MBED_CONF_MBED_TRACE_ENABLE
    mbed_trace_init();
#endif

    SocketDemo *example = new SocketDemo();
    MBED_ASSERT(example);
    example->run();

    return 0;
}
