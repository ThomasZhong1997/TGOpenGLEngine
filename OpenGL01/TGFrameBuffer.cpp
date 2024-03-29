﻿#include "TGFrameBuffer.h"
#include <iostream>

TGFrameBuffer::TGFrameBuffer(int scrWidth, int scrHeight)
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// 生成位置缓冲
	glGenTextures(1, &gPositionTexture);
	glBindTexture(GL_TEXTURE_2D, gPositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionTexture, 0);

	glGenTextures(1, &gNormalTexture);
	glBindTexture(GL_TEXTURE_2D, gNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scrWidth, scrHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalTexture, 0);

	glGenTextures(1, &gAlbedoSpecTexture);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scrWidth, scrHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecTexture, 0);

	glGenTextures(1, &gMaterialParamTexture);
	glBindTexture(GL_TEXTURE_2D, gMaterialParamTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scrWidth, scrHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMaterialParamTexture, 0);

	glGenTextures(1, &gDepthTexture);
	glBindTexture(GL_TEXTURE_2D, gDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, scrWidth, scrHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gDepthTexture, 0);

	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);

	glGenRenderbuffers(1, &gDepthStencilRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepthStencilRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scrWidth, scrHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthStencilRenderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	else
	{
		bIsFrameBufferComplete = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mBufferHeight = scrHeight;
	mBufferWidth = scrWidth;
}

void TGFrameBuffer::Use()
{
	if (!bIsFrameBufferComplete)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);
}

void TGFrameBuffer::EndUse()
{
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int TGFrameBuffer::GetGPositionTexture()
{
	return gPositionTexture;
}

unsigned int TGFrameBuffer::GetGNormalTexture()
{
	return gNormalTexture;
}

unsigned int TGFrameBuffer::GetGAlbedoSpecTexture()
{
	return gAlbedoSpecTexture;
}

unsigned int TGFrameBuffer::GetGRenderBuffer()
{
	return gDepthStencilRenderBuffer;
}

