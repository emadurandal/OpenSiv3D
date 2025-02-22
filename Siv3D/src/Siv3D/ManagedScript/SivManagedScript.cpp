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

# include <Siv3D/ManagedScript.hpp>
# include <Siv3D/ManagedScript/ManagedScriptDetail.hpp>

namespace s3d
{
	ManagedScript::ManagedScript()
		: pImpl{ std::make_shared<ManagedScriptDetail>() } {}

	ManagedScript::ManagedScript(const FilePathView path)
		: pImpl{ std::make_shared<ManagedScriptDetail>(path) } {}

	ManagedScript::~ManagedScript() {}

	bool ManagedScript::isEmpty() const
	{
		return pImpl->isEmpty();
	}

	ManagedScript::operator bool() const
	{
		return (not pImpl->isEmpty());
	}

	bool ManagedScript::compiled() const
	{
		return pImpl->compiled();
	}

	void ManagedScript::run() const
	{
		return pImpl->run();
	}
}
