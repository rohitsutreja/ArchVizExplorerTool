#pragma once

#include "CoreMinimal.h"
#include "Managers/ArchVizManager.h"
#include "RoadConstructionManager.generated.h"

class URoadConstructionWidget;
class ARoadSplineActor;

UCLASS(Blueprintable)
class ARCHVIZ_API  URoadConstructionManager : public UArchVizManager
{
	GENERATED_BODY()

public:

	UPROPERTY()
	URoadConstructionWidget* RoadConstructionUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoadConstructionWidget> RoadConstructionUIClass;


	UPROPERTY()
	ARoadSplineActor* CurrentRoadSpline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadSplineActor> RoadSplineClass;


	TArray<ARoadSplineActor*> ArrayOfRoads;


	void CreateNewRoad();
	void SaveCurrentRoad();


	UFUNCTION()
	void HandleButtonClick();


	virtual void Start() override;
	virtual void SetUp() override;
	virtual void End() override;

	void OnLeftClick();
	void SelectRoadUnderCursor();
};
