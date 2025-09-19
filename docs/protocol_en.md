 ## 1. Device Address Configuration

 The device supports dual addressing. In addition to the universal address, the second address can be switched by long-pressing the button. The correspondence between the address indicator light and the address is as follows:

 | Device Address (Hex) | Color Identifier  |
 | :------------------: | :---------------: |
 |       **0x6F**       | Universal Address |
 |       **0x71**       |        Red        |
 |       **0x72**       |       Green       |
 |       **0x73**       |       Blue        |
 |       **0x74**       |      Purple       |

------

 ## 2. Core Functionality

 ### 1. Training and Recognition Characteristics

 - **Automatic Threshold Recording**: After completing color/grayscale/binary training, the system automatically saves the current threshold parameters to non-volatile storage without requiring manual storage.
 - **Training Integrity Requirement**: During training, all color channels (or corresponding ports) must be effectively scanned. If any channel is not scanned, the recognition accuracy of that channel will be significantly reduced (resulting in potential misjudgments or missed detections).

------

 ## 3. Communication Protocol Specifications

 ### 1. Command Settings (Control Instructions)

 Control the module's operating mode by writing a single-byte command (0x00–0x15). The specific definitions are as follows:

 | Command ID (Decimal) |                     Function Description                     |
 | -------------------: | :----------------------------------------------------------: |
 |                    0 | Idle Mode (No operation; the sensor remains in standby state) |
 |                    1 | Color Recognition Mode (Detects and outputs the current target color type) |
 |                    2 | Grayscale Recognition Mode (Detects and outputs the current target grayscale level) |
 |                    3 | Binary Recognition Mode (Detects and outputs the current target binary state) |
 |                    4 | Grayscale Learning Mode (Enters grayscale threshold training state) |
 |                    5 | Binary Learning Mode (Enters binary threshold training state) |
 |                    6 | Clear All Stored Color Learning Data (Restores default thresholds) |
 |                    7 | Learn Red Threshold (Trains the threshold for the red channel independently) |
 |                    8 | Learn Green Threshold (Trains the threshold for the green channel independently) |
 |                    9 | Learn Blue Threshold (Trains the threshold for the blue channel independently) |
 |                   10 | Learn Yellow Threshold (Trains the threshold for the yellow mixed color) |
 |                   11 | Learn Cyan Threshold (Trains the threshold for the cyan mixed color) |
 |                   12 | Learn Purple Threshold (Trains the threshold for the purple mixed color) |
 |                   13 | <font color='red'>Reserved (Function not yet available)</font> :sweat: |
 |                   14 | <font color='red'>Reserved (Function not yet available)</font> :sweat: |
 |                   15 | Read Raw Photosensor Value (Outputs an analog value in the range of 0–255) |

 > **Key Constraint**: Only **1 byte of command** can be written per communication session. Writing multiple bytes (e.g., accidentally sending multiple commands) will cause the device to crash. Strict adherence to the single-byte write rule is required.

------

 ### 2. Reading Specifications (Status Feedback)

 When responding to a read request, the module returns a fixed **7-byte data packet** with the following structure:

 | Byte Position |                     Data Content                      |                         Description                          |
 | ------------: | :---------------------------------------------------: | :----------------------------------------------------------: |
 |     Bytes 1–6 | Per-Channel Detection Values (6 Independent Channels) | Each byte corresponds to the recognition result of an input signal channel (specific meanings refer to the "Color Recognition Read Values" table). |
 |        Byte 7 |               Module Status Identifier                | Indicates the current operating mode of the module (consistent with the identifiers in the "Command Settings" table, used to confirm successful mode switching). |

 > **Mandatory Requirement**: **All 7 bytes must be read in full**. Reading fewer bytes (e.g., only 6 bytes) may disrupt the device's internal state machine, leading to crashes or data anomalies.
 >
 > **Timing Note**: Due to the internal processing delay required for mode switching, immediately reading after setting a new command may return residual data from the previous mode. It is recommended to add an appropriate software delay (e.g., 50–100 ms) after writing a command to ensure the module completes the mode transition before executing a read operation.

------

 ### 3. Color Recognition Read Value Mapping Table

 When the module operates in Color Recognition Mode (Command 1) or related learning modes, the first 6 bytes of the returned data correspond to the color types of each channel. The specific mappings are as follows:

 |        Color Type | Return Identifier (Decimal) |
 | ----------------: | :-------------------------: |
 | No Color Detected |              0              |
 |               Red |              1              |
 |             Green |              2              |
 |              Blue |              3              |
 |            Yellow |              4              |
 |              Cyan |              5              |
 |            Purple |              6              |
 |             Black |              7              |
 |             White |              8              |

 > **Note**: These identifiers directly map the sensor's classification results for target colors and can be used for subsequent logic judgments or display outputs.

------

 ## 4. Typical Application Constraints

 1. **Write Safety**: Strictly limit communication to **1 byte of command per session**. Multi-byte writes (e.g., batch command + parameter transmission) must be split into multiple single-byte operations at the application layer.
 2. **Read Integrity**: All status reads must retrieve **exactly 7 bytes**; truncating or partially reading the data is prohibited.
 3. **Timing Control**: Add a software delay (recommended: 50–100 ms) after mode switching to avoid data errors caused by sensor response latency.
 4. **Training Validity**: When performing color/grayscale/binary learning, ensure stable ambient lighting and comprehensive coverage of the target color; otherwise, the threshold training may yield inaccurate results.

------

 *This protocol is designed for precise control and data interaction with multi-channel color sensors. Users must strictly adhere to communication timing and data format requirements to ensure system stability and recognition accuracy.*