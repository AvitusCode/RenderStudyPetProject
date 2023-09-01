#include "Render/FrameBuffer.h"
#include "Display.h"
#include "Utils/logger.h"

FrameBuffer::~FrameBuffer() noexcept {
    deleteFrameBuffer();
}

bool FrameBuffer::createFrameBufferCD(const GLsizei width, const GLsizei height)
{
    if (frameBufferID_) {
        return false;
    }

    glGenFramebuffers(1, &frameBufferID_);
    if (!frameBufferID_)
    {
        LOG(ERROR) << "Unable to create framebuffer!" << std::endl;
        return false;
    }

    width_ = width;
    height_ = height;

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
    LOG(INFO) << "Created framebuffer with ID: " << frameBufferID_ << ", its dimensions will be [" << width << ", " << height << "]" << std::endl;

    if (!withColorAttachment(GL_RGBA8) || !withDepthAttachment(GL_DEPTH_COMPONENT24)) {
        return false;
    }

    return finishInitialization();
}

void FrameBuffer::bindAsBothReadAndDraw() const {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
}

void FrameBuffer::bindAsRead() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferID_);
}

void FrameBuffer::bindAsDraw() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferID_);
}

bool FrameBuffer::resize(GLsizei newWidth, GLsizei newHeight)
{
    if (frameBufferID_ == 0) {
        return false;
    }

    deleteOnlyFrameBuffer();
    // First create and bind framebuffer object (FBO)
    glGenFramebuffers(1, &frameBufferID_);
    if (frameBufferID_ == 0)
    {
        LOG(ERROR) << "Unable to create framebuffer during resizing!" << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
    LOG(INFO) << "Resizing framebuffer with ID: " << frameBufferID_ << ", its dimensions will be [" << newWidth << ", " << newHeight << "]" << std::endl;

    if (colorRenderBuffer_)
    {
        if (!colorRenderBuffer_->resize(newWidth, newHeight))
        {
            LOG(ERROR) << "Unable to resize color attachment for the framebuffer: " << frameBufferID_ << std::endl;
            deleteFrameBuffer();
            return false;
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer_->getID());
    }

    if (depthRenderBuffer_)
    {
        if (!depthRenderBuffer_->resize(newWidth, newHeight))
        {
            LOG(ERROR) << "Unable to resize depth attachment for the framebuffer: " << frameBufferID_ << std::endl;
            deleteFrameBuffer();
            return false;
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_->getID());
    }

    if (texture_)
    {
        if (!texture_->resize(newWidth, newHeight))
        {
            LOG(ERROR) << "Unable to resize depth attachment for the framebuffer: " << frameBufferID_ << std::endl;
            deleteFrameBuffer();
            return false;
        }

        texture_->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->getID(), 0);
    }

    // Check FBO status when all attachments have been attached
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    width_ = newWidth;
    height_ = newHeight;

    return true;
}

void FrameBuffer::setFullViewport() const {
    glViewport(0, 0, width_, height_);
}

void FrameBuffer::copyColorToDefaultFrameBuffer() const
{
    const auto screenWidth = Display::getDisplay().getWidth();
    const auto screenHeight = Display::getDisplay().getHeight();

    bindAsRead();
    Default::bindAsDraw();
    glBlitFramebuffer(0, 0, width_, height_, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::copyDepthFromDefaultFrameBuffer() const
{
    const auto screenWidth = Display::getDisplay().getWidth();
    const auto screenHeight = Display::getDisplay().getHeight();

    Default::bindAsRead();
    bindAsDraw();

    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, width_, height_, GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

GLsizei FrameBuffer::getWidth() const {
    return width_;
}

GLsizei FrameBuffer::getHeight() const {
    return height_;
}

GLint FrameBuffer::getDepthBits()
{
    if (depthBits_ != -1 || frameBufferID_ == 0) {
        return depthBits_;
    }

    bindAsRead();
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depthBits_);
    if (const auto error = glGetError(); error != GL_NO_ERROR)
    {
        LOG(ERROR) << "Could not read number of depth bits for framebuffer: " << frameBufferID_ << " (error " << error << ")! "
            << "Probably it has no depth attachment!" << std::endl;
    }

    return depthBits_;
}

GLint FrameBuffer::getStencilBits()
{
    if (stencilBits_ != -1 || frameBufferID_ == 0) {
        return stencilBits_;
    }

    bindAsRead();
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencilBits_);
    if (const auto error = glGetError(); error != GL_NO_ERROR)
    {
        LOG(ERROR) << "Could not read number of stencil bits for framebuffer: " << frameBufferID_ << " (error " << error << ")! "
            << "Probably it has no stencil attachment!" << std::endl;
    }

    return stencilBits_;
}

std::array<GLubyte, 4> FrameBuffer::readColorValue(int x, int y)
{
    std::array<GLubyte, 4> result; result.fill(0);
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, result.data());
    return result;
}

void FrameBuffer::deleteFrameBuffer() noexcept
{
    colorRenderBuffer_.reset();
    depthRenderBuffer_.reset();
    deleteOnlyFrameBuffer();
}

Texture* FrameBuffer::getTexture() const
{
    return texture_.get();
}

void FrameBuffer::Default::bindAsBothReadAndDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsRead() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsDraw() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLint FrameBuffer::Default::getDepthBits()
{
    GLint depthBits{ -1 };
    bindAsRead();
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depthBits);
    return depthBits;
}

GLint FrameBuffer::Default::getStencilBits()
{
    GLint stencilBits{ -1 };
    bindAsRead();
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencilBits);
    return stencilBits;
}

void FrameBuffer::Default::setFullViewport()
{
    const auto screenWidth = Display::getDisplay().getWidth();
    const auto screenHeight = Display::getDisplay().getHeight();

    glViewport(0, 0, screenWidth, screenHeight);
}

FrameBuffer::Builder& FrameBuffer::Builder::createAndBind(GLsizei width, GLsizei height)
{
    frameBuffer_ = std::make_unique<FrameBuffer>();
    frameBuffer_->createAndBind(width, height);
    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withColorAttachment(GLenum internalFormat)
{
    if (frameBuffer_) {
        frameBuffer_->withColorAttachment(internalFormat);
    }

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withTextureColorAttachment(GLenum internalFormat)
{
    if (frameBuffer_) {
        frameBuffer_->withTextureColorAttachment(internalFormat);
    }

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withDepthAttachment(GLenum internalFormat)
{
    if (frameBuffer_) {
        frameBuffer_->withDepthAttachment(internalFormat);
    }

    return *this;
}

std::unique_ptr<FrameBuffer> FrameBuffer::Builder::finishAndGetUnique()
{
    if (frameBuffer_ == nullptr) {
        return nullptr;
    }

    return frameBuffer_->finishInitialization() ? std::move(frameBuffer_) : nullptr;
}

std::shared_ptr<FrameBuffer> FrameBuffer::Builder::finishAndGetShared()
{
    if (frameBuffer_ == nullptr) {
        return nullptr;
    }

    return frameBuffer_->finishInitialization() ? std::move(frameBuffer_) : nullptr;
}

bool FrameBuffer::createAndBind(GLsizei width, GLsizei height)
{
    glGenFramebuffers(1, &frameBufferID_);
    if (frameBufferID_ == 0)
    {
        LOG(ERROR) << "Unable to create framebuffer!" << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
    LOG(INFO) << "Created framebuffer with ID: " << frameBufferID_ << ", its dimensions will be [" << width << ", " << height << "]" << std::endl;
    width_ = width;
    height_ = height;

    return true;
}

bool FrameBuffer::withColorAttachment(GLenum internalFormat)
{
    if (!frameBufferID_) {
        return false;
    }

    // Create color render buffer and attach it to FBO
    auto colorRenderBuffer = std::make_unique<RenderBuffer>();
    if (!colorRenderBuffer->create(internalFormat, width_, height_))
    {
        LOG(ERROR) << "Unable to create color attachment for the framebuffer: " << frameBufferID_ << std::endl;
        deleteFrameBuffer();
        return false;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer->getID());
    colorRenderBuffer_ = std::move(colorRenderBuffer);
    return true;
}

bool FrameBuffer::withTextureColorAttachment(GLenum internalFormat)
{
    if (!frameBufferID_) {
        return false;
    }

    // Create an empty texture with no data and same size as framebuffer has
    texture_ = std::make_unique<Texture>();
    texture_->createFromData(nullptr, width_, height_, internalFormat, false);
    texture_->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->getID(), 0);
    return true;
}

bool FrameBuffer::withDepthAttachment(GLenum internalFormat)
{
    // Create depth buffer and attach it to FBO
    auto depthRenderBuffer = std::make_unique<RenderBuffer>();
    if (!depthRenderBuffer->create(internalFormat, width_, height_))
    {
        LOG(ERROR) << "Unable to create depth attachment for the framebuffer: " << frameBufferID_ << std::endl;
        deleteFrameBuffer();
        return false;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer->getID());
    depthRenderBuffer_ = std::move(depthRenderBuffer);
    return true;
}

bool FrameBuffer::finishInitialization() const
{
    if (!frameBufferID_) {
        return false;
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}

void FrameBuffer::deleteOnlyFrameBuffer() noexcept
{
    if (frameBufferID_ == 0) {
        return;
    }

    LOG(INFO) << "Deleting framebuffer with ID: " << frameBufferID_ << std::endl;
    glDeleteFramebuffers(1, &frameBufferID_);
    height_ = width_ = frameBufferID_ = 0;
    stencilBits_ = depthBits_ = -1;
}