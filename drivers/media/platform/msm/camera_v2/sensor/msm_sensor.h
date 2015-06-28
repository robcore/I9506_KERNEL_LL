/* Copyright (c) 2011-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef MSM_SENSOR_H
#define MSM_SENSOR_H

#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <mach/camera2.h>
#include <media/msm_cam_sensor.h>
#include <media/v4l2-subdev.h>
#include "msm_camera_i2c.h"
#include "msm_camera_dt_util.h"
#include "msm_sd.h"

#define DEFINE_MSM_MUTEX(mutexname) \
	static struct mutex mutexname = __MUTEX_INITIALIZER(mutexname)

struct msm_sensor_ctrl_t;

enum msm_sensor_state_t {
	MSM_SENSOR_POWER_DOWN,
	MSM_SENSOR_POWER_UP,
};
struct msm_sensor_fn_t {
	int (*sensor_config) (struct msm_sensor_ctrl_t *, void __user *);
	int (*sensor_power_down) (struct msm_sensor_ctrl_t *,
		struct msm_camera_power_ctrl_t *,	
		enum msm_camera_device_type_t, struct msm_camera_i2c_client *);
	int (*sensor_power_up) (struct msm_sensor_ctrl_t *,
		struct msm_camera_power_ctrl_t *,
		struct msm_camera_i2c_client *,
		struct msm_camera_slave_info *, const char *);
	int (*sensor_match_id) (struct msm_camera_i2c_client *,
		struct msm_camera_slave_info *, const char *);
};

struct msm_sensor_ctrl_t {
	struct platform_device *pdev;
	struct mutex *msm_sensor_mutex;
	enum msm_camera_device_type_t sensor_device_type;
	struct msm_camera_sensor_board_info *sensordata;
	struct msm_sensor_packed_cfg_t *cfg_override;
	struct msm_sd_subdev msm_sd;
	enum cci_i2c_master_t cci_i2c_master;

	struct msm_camera_i2c_client *sensor_i2c_client;
	struct device *dev;

	struct v4l2_subdev_info *sensor_v4l2_subdev_info;
	uint8_t sensor_v4l2_subdev_info_size;
	struct v4l2_subdev_ops *sensor_v4l2_subdev_ops;
	struct msm_sensor_fn_t *func_tbl;
	struct msm_camera_i2c_reg_setting stop_setting;
	void *misc_regulator;
	enum msm_sensor_state_t sensor_state;
	uint8_t is_probe_succeed;
};

int msm_sensor_config(struct msm_sensor_ctrl_t *s_ctrl, void __user *argp);

int msm_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl,
	struct msm_camera_power_ctrl_t *power_info,
	struct msm_camera_i2c_client *sensor_i2c_client,
	struct msm_camera_slave_info *slave_info,
	const char *sensor_name);

int msm_sensor_power_down(struct msm_sensor_ctrl_t *s_ctrl,
	struct msm_camera_power_ctrl_t *,
	enum msm_camera_device_type_t, struct msm_camera_i2c_client *);

int msm_sensor_check_id(struct msm_sensor_ctrl_t *,
	struct msm_camera_i2c_client *,
	struct msm_camera_slave_info *,
	const char *);
int msm_sensor_match_id(struct msm_camera_i2c_client *,
	struct msm_camera_slave_info *,
	const char *);

int msm_sensor_update_cfg(struct msm_sensor_ctrl_t *s_ctrl);
int msm_sensor_platform_probe(struct platform_device *pdev,
	void *data);

int msm_sensor_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id, struct msm_sensor_ctrl_t *s_ctrl);

int msm_sensor_free_sensor_data(struct msm_sensor_ctrl_t *s_ctrl);

int32_t msm_sensor_init_default_params(struct msm_sensor_ctrl_t *s_ctrl);
#endif
