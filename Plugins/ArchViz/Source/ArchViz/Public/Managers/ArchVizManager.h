#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.generated.h"

class AArchVizController;

UCLASS(Abstract)
class ARCHVIZ_API UArchVizManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void SetUp();
	virtual void Start() PURE_VIRTUAL(UArchVizManager::Start, );
	virtual void End() PURE_VIRTUAL(UArchVizManager::End, );

protected:
	UPROPERTY()
	AArchVizController* Controller;

};
