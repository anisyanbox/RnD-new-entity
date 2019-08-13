# Fligth with ARM TrustZone technology

Today every mobile devices based on ARM-chip there are two operating systems.
One of its so called Rich-OS (or `REE`) and other `TEE`.
 - REE - Rich Execution Environment. It wiil be any host OS. For example the Android or the pure Linux.
 - TEE - Trusted Execution Environment. It is a secure OS. It can do any secure operation in separate space.
For example: calculating any hash, sign a file, verify a signed file and so on.

This scheme is applied today for secure mobile operations (online banking, person identification, DRM, protection against spoofing information and files).
As a safe operating system may be [Trusty TEE](https://source.android.com/security/trusty) from Google for Android or any open source decision.
It's very actual today, because any modern opeating systems contain a lot of [vulnerabilities](https://www.cvedetails.com/vulnerability-list/vendor_id-33/product_id-47/cvssscoremin-7/cvssscoremax-7.99/Linux-Linux-Kernel.html).

I have decided to use open source trusted OS [OP-TEE](https://optee.readthedocs.io/index.html) and my smartphone based on ARM MTK chip with SailfishOS on board (Linux kernel based). 
ARM core maintains TEE-technology. It has marketing name `TrustZone`. You can read awesome [docmentation](http://infocenter.arm.com/help/topic/com.arm.doc.prd29-genc-009492c/PRD29-GENC-009492C_trustzone_security_whitepaper.pdf) on it.

Any host application can be a client of trust zone and do request for action.
If you want to use some trusted application you should do some additional building and installation:
 - [ARM-toolchain](https://www.acmesystems.it/arm9_toolchain) - Cross compiller for ARM platform.
 - [optee_client](https://github.com/OP-TEE/optee_client) - There you can build TEE Client API.
 - [TEEE_Client_API](https://optee.readthedocs.io/architecture/globalplatform_api.html#tee-internal-core-api) - It is "how to use" installed optee client.

### Few words about how it works:
Note: It is actually only for ARM platforms with TrustZone with OP-TEE.

1. Your application uses the TEEE_Client_API.
2. Any functions from the API uses [SMC-calls](http://infocenter.arm.com/help/topic/com.arm.doc.den0028b/ARM_DEN0028B_SMC_Calling_Convention.pdf#page=7&zoom=100,0,172) that transfer the ARM-processor to EL3 mode (the highest privileged mode).
3. ?????????????????
4. PROFFFIT !!! There secure code in OP-TEE starts execution.
5. Secure OS itself swap the processor to normal world.

In my case secure OS have been flashed already in my device. But you can do it yourself from [optee-sources](https://github.com/OP-TEE/optee_os) and with help any flashing tool for your device.

### How to install all:
After you are convinced that you have ARM based targer with flashed TrustZone, you should:
1. Learn how to [install](https://optee.readthedocs.io/architecture/trusted_applications.html) your TA in TrustZone.
2. With help `scp` upload your host-app to target.
3. There just run your REE binary.
```console
[root@Sailfish nemo]# ./ree 12 -12
Invoking TA to multiply some values:
12 * -12 = -144
```

### Links that have been used:
 - [OP-TEE-Sample-Applications](https://github.com/linaro-swg/optee_examples)
 - [Optee_examples](https://optee.readthedocs.io/building/gits/optee_examples/optee_examples.html)
