import ctypes

# Define the structure for BME280_TempData
class BME280_TempData(ctypes.Structure):
    _fields_ = [
        ("t_fine", ctypes.c_int32),
        ("temperature", ctypes.c_double)
    ]

class BME280CalibData:
    def __init__(self, dig_T1, dig_T2, dig_T3, dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6,
                 dig_P7, dig_P8, dig_P9, dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6):
        # Temperature calibration parameters
        self.dig_T1 = dig_T1  # uint16_t
        self.dig_T2 = dig_T2  # int16_t
        self.dig_T3 = dig_T3  # int16_t
        
        # Pressure calibration parameters
        self.dig_P1 = dig_P1  # uint16_t
        self.dig_P2 = dig_P2  # int16_t
        self.dig_P3 = dig_P3  # int16_t
        self.dig_P4 = dig_P4  # int16_t
        self.dig_P5 = dig_P5  # int16_t
        self.dig_P6 = dig_P6  # int16_t
        self.dig_P7 = dig_P7  # int16_t
        self.dig_P8 = dig_P8  # int16_t
        self.dig_P9 = dig_P9  # int16_t
        
        # Humidity calibration parameters
        self.dig_H1 = dig_H1  # uint8_t
        self.dig_H2 = dig_H2  # int16_t
        self.dig_H3 = dig_H3  # uint8_t
        self.dig_H4 = dig_H4  # int16_t
        self.dig_H5 = dig_H5  # int16_t
        self.dig_H6 = dig_H6  # int8_t
        

class interpreter_interface:
    def __init__(self, lib_path, calib_data):
        # Load the shared library
        self.lib = ctypes.CDLL(lib_path)

        # Calibration data
        self.calib_data = calib_data

        # Define the return types and argument types for the functions you want to call
        self._setup_library_functions()

    def _setup_library_functions(self):
        # Setup argument and return types for functions in the shared library

        # convertToAccel function setup
        self.lib.convertToAccel.argtypes = [ctypes.c_int16]  # input is int16_t
        self.lib.convertToAccel.restype = ctypes.c_float     # output is float

        # compensate_T function setup
        self.lib.compensate_T.argtypes = [ctypes.c_int32, ctypes.c_uint16, ctypes.c_int16, ctypes.c_int16]
        self.lib.compensate_T.restype = BME280_TempData

        # compensate_P function setup
        self.lib.compensate_P.argtypes = [
            ctypes.c_int32, ctypes.c_int32,
            ctypes.c_uint16, ctypes.c_int16, ctypes.c_int16, ctypes.c_int16,
            ctypes.c_int16, ctypes.c_int16, ctypes.c_int16, ctypes.c_int16
        ]
        self.lib.compensate_P.restype = ctypes.c_double  # output is double

        # compensate_H function setup
        self.lib.compensate_H.argtypes = [
            ctypes.c_int32, ctypes.c_uint32, ctypes.c_uint8,
            ctypes.c_int16, ctypes.c_uint8, ctypes.c_int16, ctypes.c_int16, ctypes.c_int8
        ]
        self.lib.compensate_H.restype = ctypes.c_double  # output is double
        
    def convert_to_accel(self, raw_accel):
        return self.lib.convertToAccel(raw_accel)

    def compensate_temperature(self, adc_T):
        return self.lib.compensate_T(adc_T, self.calib_data.dig_T1, self.calib_data.dig_T2, self.calib_data.dig_T3)

    def compensate_pressure(self, adc_P, t_fine):
        return self.lib.compensate_P(t_fine, adc_P, self.calib_data.dig_P1, self.calib_data.dig_P2, self.calib_data.dig_P3,
                                    self.calib_data.dig_P4, self.calib_data.dig_P5, self.calib_data.dig_P6, self.calib_data.dig_P7,
                                    self.calib_data.dig_P8, self.calib_data.dig_P9)

    def compensate_humidity(self, adc_H, t_fine):
        return self.lib.compensate_H(t_fine, adc_H, self.calib_data.dig_H1, self.calib_data.dig_H2, self.calib_data.dig_H3,
                                    self.calib_data.dig_H4, self.calib_data.dig_H5, self.calib_data.dig_H6)
