
using UnrealBuildTool;
using System.Collections.Generic;

public class MastermindEditorTarget : TargetRules
{
	public MastermindEditorTarget(TargetInfo Target) : base(Target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Mastermind");
	}
}
