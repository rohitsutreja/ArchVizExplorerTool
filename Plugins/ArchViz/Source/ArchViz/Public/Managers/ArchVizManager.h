#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.generated.h"

class AArchVizController;

UCLASS(Abstract)
class ARCHVIZ_API UArchVizManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AArchVizController* Controller;

	virtual void SetUp() PURE_VIRTUAL(UArchVizManager::SetUp, );
	virtual void Start() PURE_VIRTUAL(UArchVizManager::Start, );
	virtual void End() PURE_VIRTUAL(UArchVizManager::End, );
};
