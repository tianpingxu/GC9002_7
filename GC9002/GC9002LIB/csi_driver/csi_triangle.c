#include <csi_triangle.h>
#include <soft_libc.h>
#ifdef CONFIG_CSI_DRV_TRIANGLE_ENABLED
/**
  * @brief  angle_change_value_fixed
  * @param  angle Angle value, Fixed-point integer: 1.7.24
  *         eg:sin(30),angle = 30*2^24(30<<24);
  *            sin(30.5),angle = 30.5*2^24
  * @retval result Fixed-point integer: 1.15.16, eg:sin(30), result=0.5*2^16
  */
static int64_t angle_change_value_fixed(int64_t angle)
{

    if(angle < 0)
    {
        while(angle < 0)
        {
            angle += ((int64_t)360<<24);
        }
    }
    else if(angle >= ((int64_t)360<<24))
    {
        while(angle >= ((int64_t)360<<24))
        {
            angle -= ((int64_t)360<<24);
        }
    }
    // now , angle >= 0, and angle < (360<<24)
    return angle;
}
/**
  * @brief  sin Fixed-point mode
  * @param  angle Angle value, Fixed-point integer: 1.7.24
  *         eg:sin(30),angle = 30*2^24(30<<24);
  *            sin(30.5),angle = 30.5*2^24
  * @retval result Fixed-point integer: 1.15.16, eg:sin(30), result=0.5*2^16
  */
int csi_sin_fixed(int64_t angle)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    int sign = 1;
    int64_t angle_ch = angle_change_value_fixed(angle);
    //sin(360-a) = -sin(a)
    if(angle_ch > ((int64_t)180<<24))
    {
        sign = -1;
        angle_ch = ((int64_t)360<<24)-angle_ch;
    }
    //sin(180-a) = sin(a)
    if(angle_ch > (90<<24) && angle_ch <= ((int64_t)180<<24))
    {
        angle_ch = (int64_t)((int64_t)180<<24) - angle_ch;
    }
    // now , angle_ch >= 0, angle_ch <= (90<<24)
    x_i = 0x10000;
    y_i = 0;
    z_i = (uint32_t)angle_ch;
    HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return y_o*sign;
}
/**
  * @brief  cos Fixed-point mode
  * @param  angle Angle value, Fixed-point integer: 1.39.24
  *         eg:cos(30),angle = 30*2^24(30<<24);
  *            cos(30.5),angle = 30.5*2^24
  * @retval result Fixed-point integer: 1.15.16, 
  *         eg:cos(60), result=0.5*2^16
  */
int csi_cos_fixed(int64_t angle)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    int sign = 1;
    int64_t angle_ch = angle_change_value_fixed(angle);
    //cos(360-a) = cos(a)
    if(angle_ch > (int64_t)((int64_t)180<<24))
    {
        angle_ch = (int64_t)((int64_t)360<<24) - angle_ch;
    }
    //cos(180-a) = -cos(a)
    if(angle_ch > (90<<24) && angle_ch <= (int64_t)((int64_t)180<<24))
    {
        sign = -1;
        angle_ch = ((int64_t)((int64_t)180<<24) - angle_ch);
    }
    // now , angle_ch >= 0, angle_ch <= (90<<24)
    x_i = 0x10000;
    y_i = 0;
    z_i = (uint32_t)angle_ch;
    HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return x_o*sign;
}
/**
  * @brief  arctan Fixed-point mode
  * @param  tan tangent value, Fixed-point integer: 1.15.16
  *         eg:arctan(1),val = 1*2^16(1<<16);
  *            arctan(1.5),val = 1.5*2^16
  * @retval result Fixed-point integer: 1.7.24, 
  *         eg:arctan(1), result=45*2^24
  */
int csi_arctan_fixed(int tan)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    x_i = 0x10000;
    y_i = tan;
    z_i = 0;
    HAL_EXTENSION_CordicVectoring(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return z_o;
}
#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE)
static float angle_change_value(float angle)
{
    if(angle < 0.0)
    {
        while(angle < 0.0)
        {
            angle += 360;
        }
    }
    else if(angle >= 360)
    {
        while(angle >= 360)
        {
            angle -= 360;
        }
    }
    // now , angle >= 0, and angle < 360
    return angle;
}
/**
  * @brief  sin Float-point mode
  * @param  angle Arbitrary Angle value
  * @retval result
  */
float csi_sin(float angle)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    int sign = 1;
    int angle_int = 0;
    float angle_ch =  angle_change_value(angle);
    // now , angle_ch >= 0, angle_ch < 360
    //sin(360-a) = -sin(a)
    if(angle_ch > 180)
    {
        sign = -1;
        angle_ch = 360.0-angle_ch;
    }
    //sin(180-a) = sin(a)
    if(angle_ch > 90 && angle_ch <= 180)
    {
        angle_ch = 180.0-angle_ch;
    }
    // now , angle_ch >= 0, angle_ch <= 90
    angle_int = float_to_int(angle_ch, 24);
    x_i = 0x10000;
    y_i = 0;
    z_i = angle_int;
    HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return int_to_float(y_o, 16)*sign;
}
/**
  * @brief  cos Float-point mode
  * @param  angle Arbitrary Angle value
  * @retval result
  */
float csi_cos(float angle)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    int sign = 1;
    int angle_int = 0;
    float angle_ch =  angle_change_value(angle);
    // now , angle_ch >= 0, angle_ch < 360
    //cos(360-a) = cos(a)
    if(angle_ch > 180)
    {
        angle_ch = 360.0-angle_ch;
    }
    //cos(180-a) = -cos(a)
    if(angle_ch > 90 && angle_ch <= 180)
    {
        sign = -1;
        angle_ch = 180.0-angle_ch;
    }
    // now , angle_ch >= 0, angle_ch <= 90
    angle_int = float_to_int(angle_ch, 24);
    x_i = 0x10000;
    y_i = 0;
    z_i = angle_int;
    HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return int_to_float(x_o, 16)*sign;
}
/**
  * @brief  cos Float-point mode
  * @param  tan tan value
  * @retval result
  */
float csi_arctan(float arc)
{
    uint32_t x_i;
    uint32_t y_i;
    uint32_t z_i;
    uint32_t x_o;
    uint32_t y_o;
    uint32_t z_o;
    x_i = 0x10000;
    y_i = float_to_int(arc, 16);
    z_i = 0;
    HAL_EXTENSION_CordicVectoring(x_i, y_i, z_i, &x_o, &y_o, &z_o);
    return int_to_float(z_o, 24);
}
#endif /* CONFIG_CSI_FLOAT_SOFT_ENABLE */

#endif