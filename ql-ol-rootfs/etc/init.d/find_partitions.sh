#!/bin/sh
# Copyright (c) 2014, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#     * Neither the name of The Linux Foundation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# find_partitions        init.d script to dynamically find partitions
#

MtdProc="/proc/mtd"
UbiCtrl="/dev/ubi_ctrl"

# $2   N  0.01sec
WaitDeviceCreate() {
	local device=${1}
	local timeout=${2}
	local count="0"
    [ -z "${timeout}" ] && timeout="200"
	while [ ! -e ${device} ]
	do
        sleep 0.01
        count=$((count+1))
        [ ${count} -gt ${timeout} ] && return 1
	done
    return 0
}

checkMountResult() {
    local device="${1}"
    local mountpoint="${2}"
	local mountResult1=`mount | grep $device | grep $mountpoint`

	if [ "" = "$mountResult1" ];then
		echo "$device mount to $mountpoint fail !!!!"
		return 1
	else
		echo "$device mount to $mountpoint success !!!!"
		return 0
	fi
}

MountFS() {
    local fstype="${1}"
    local device="${2}"
    local mountpoint="${3}"
    local mountflags="${4}"
    mkdir -p ${mountpoint}
    WaitDeviceCreate "${device}"
    if [ $? -eq 0 ];then
        mount -t ${fstype} ${device} ${mountpoint} -o ${mountflags}
    else
        echo "${device} is not exist Can't mount ${device} in ${mountpoint}"
    fi

	sleep 0.5
	checkMountResult "${device}" "${mountpoint}"
	if [ $? -eq 0 ];then
		return 0
	else
		return 1
	fi
}

FindAndAttachUBI() {
    local partition=${1}
    local ubinum=${2}
    if [ ! -e "/dev/ubi${ubinum}" ];then
        mtdnum=`grep "\<${partition}\>" ${MtdProc} | sed 's/mtd\([-1-9]*\): .*/\1/g'`
        echo "MTD : Detected block device : $partition mtd$mtdnum ubi$ubinum"
        ubiattach -m ${mtdnum} -d ${ubinum} ${UbiCtrl}
    fi
}

# The Function only mount ubi volume 0(!!!)
FindAndMountUBI() {
    local partition=${1}
    local mountpoint=${2}
    local ubinum=${3}
    local mountflags=${4}

    FindAndAttachUBI ${partition} ${ubinum}
    device="/dev/ubi${ubinum}_0"
	WaitDeviceCreate "$device"
    [ -z "${mountflags}" ] && mountflags="bulk_read"
    MountFS "ubifs" "${device}" "${mountpoint}" "${mountflags}"
	if [ $? -eq 0 ];then
		return 0
	else
		return 1
	fi
}

FindAndMountVolume() {
    local ubi_volume_dev=${1}
    local mountpoint=${2}
    local mountflags=${3}

    device="/dev/${ubi_volume_dev}"
	WaitDeviceCreate "$device"
    [ -z "${mountflags}" ] && mountflags="bulk_read"
    MountFS "ubifs" "${device}" "${mountpoint}" "${mountflags}"
	if [ $? -eq 0 ];then
		return 0
	else
		return 1
	fi
}


DetachUBI() {
    local partition=${1}
    local ubinum=${2}
		
	mtd_block_number=`grep "\<${partition}\>" ${MtdProc} | sed 's/mtd\([-1-9]*\): .*/\1/g'`
	ubidetach -p /dev/mtd$mtd_block_number
	if [ $? -ne 0];then
		echo "DetachUBI() ${partition} mtd${mtd_block_number} fail!!"
		return 1
	fi
	sleep 0.2

	return 0
}

FormatUBI() {
    local partition=${1}
	
	mtd_block_number=`grep "\<${partition}\>" ${MtdProc} | sed 's/mtd\([-1-9]*\): .*/\1/g'`
    ubiformat /dev/mtd$mtd_block_number -y
	if [ $? -ne 0];then
		echo "FormatUBI() ${partition} /dev/mtd${mtd_block_number} ubiformat fail!"
		return 1
	fi
    sleep 0.2
	return 0
}

FormatAndCreateUbi() {
    local partition=${1}
	local ubinub=${2}
	
	DetachUBI ${partition} ${ubinub}
	FormatUBI ${partition}
	
	FindAndAttachUBI ${partition} ${ubinub}
	WaitDeviceCreate "/dev/ubi${ubinub}"

    ubimkvol /dev/ubi${ubinub} -m -N $device
    WaitDeviceCreate "/dev/ubi${ubinub}_0"

	if [ -e "/dev/ubi${ubinub}_0" ];then
        echo  "ubimkvol success !!!"
    else
        echo "ubimkvol failed !!!"
		sys_reboot
    fi
}

SystemDataMountBind2usr_dataPartition()
{
#quectel add for mount bind usrfs
	if [ ! -f "/usrdata/data/mountcopyflag" ];then
		mkdir -p /usrdata/data
		cp -a /data/* /usrdata/data
		sync
		echo 1 > /usrdata/data/mountcopyflag
		sync
	fi
	mount --bind /usrdata/data /data
}

fstype="UBI"

#Ramos.zhang-20181019,modem mount and run in kernel
#eval FindAndMount${fstype} modem /firmware

#quectel add for usr_data partition mount 
# 1. load usr_data partition    
	FindAndMountUBI "usr_data" "/usrdata" "2"
	if [ $? -eq 0 ];then
		echo "usr_data  OK!!!!!"
	else
		echo " usr_data  partition can't mount ,need format !!!!"
		FormatAndCreateUbi "usr_data" 2
		FindAndMountVolume "ubi2_0" "/usrdata"
	fi

	SystemDataMountBind2usr_dataPartition

exit 0
