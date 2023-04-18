
using UnrealBuildTool;
using System.Collections.Generic;

public class MastermindTarget : TargetRules
{
	public MastermindTarget(TargetInfo Target) : base(Target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		bUseUnityBuild = true;
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Mastermind");
		
	}
}
