// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TopDown1 : ModuleRules
{
	public TopDown1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule",
			"Json",
			"JsonUtilities",
			"Http"
			});

		// копируем sqlite3.dll в папку Binary
		RuntimeDependencies.Add("$(BinaryOutputDir)/sqlite3.dll", Path.Combine(ModuleDirectory, "sqlite", "bin", "sqlite3.dll"));

		// добавляем билиотеку
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "sqlite", "bin", "sqlite3.lib"));

		// задержка на загрузку бибиотеки
		PublicDelayLoadDLLs.Add("sqlite3.dll");
		


	}
}
