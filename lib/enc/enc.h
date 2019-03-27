/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   enc.h
 * Author: tuxihc
 *
 * Created on 25 de marzo de 2019, 21:06
 */

#ifndef ENC_H
#define ENC_H

#ifdef __cplusplus
extern "C" {
#endif

void ENC_Init(void);
uint8_t ENC_Button(void);
uint8_t ENC_Encoder(void);
uint8_t ENC_GetButton(void);
uint8_t ENC_GetEncoder(void);

#ifdef __cplusplus
}
#endif

#endif /* ENC_H */

