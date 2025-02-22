﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <Siv3D/Common.hpp>
# include <Siv3D/Array.hpp>
# include <Siv3D/InputGroups.hpp>

namespace s3d
{
	class SIV3D_NOVTABLE ISiv3DScreenCapture
	{
	public:

		static ISiv3DScreenCapture* Create();

		virtual ~ISiv3DScreenCapture() = default;

		virtual void init() = 0;

		virtual void update() = 0;

		virtual const FilePath& getScreenshotDirectory() const = 0;

		virtual void setScreenshotDirectory(FilePath&& path) = 0;

		virtual void requestScreenCapture(FilePath&& path) = 0;

		virtual bool hasNewFrame() const = 0;

		virtual const Image& receiveScreenCapture() const = 0;

		virtual void setScreenshotShortcutKeys(const Array<InputGroup>& screenshotShortcutKeys) = 0;
	};
}
