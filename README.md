Struggle For Throne
===================

"언리얼" 공부를 진행하며 만든 간단한 데모 게임입니다. <br>
본 저장소에는 게임 에셋과 블루프린트를 제외한 게임 요소들이 업로드 되어 있습니다.

<br>

<프로젝트 개요>
===============
* 프로젝트 이름 : Throne
* 엔진 : Unreal Engine 5.4
* 제작 기간 : 2024.08.04 ~ 2024.08.28
* 개발 인원 : 1인
* 게임 플레이 : 3인칭 시점으로 던전을 탐색하는 로그라이크 형식

<br>

<소스 코드 구조>
=============
<details>
<summary> <h1>AI</h1> </summary>
<div markdown="1">

## AIController


AIController는 NPC 폰에 빙의되어 플레이어가 직접 조종하지 않아도 폰의 동작을 가능하게 합니다. 저는 BehaviorTree를 사용해 NPC의 동작을 결정하기 위해 AIController에 다음과 같은 코드를 작성했습니다.

```cpp
public:
	void RunAI();
	void StopAI();
```

AIController의 조종을 받는 폰에서 BehaviorTree의 실행을 On/Off 할 수 있도록 RunAI()와 StopAI()를 제공합니다. 빙의시 바로 AI가 작동할 수 있도록 OnPossess 가상 함수에서 RunAI를 호출하고 있습니다.

<br>

## BTDecorator_AttackInRange
UBTDecorator 클래스를 상속받아 제작했으며 AI의 공격범위 안에 Target(플레이어)가 있는지 검사하는 클래스입니다.

<br>

## BTService_Detect
UBTService 클래스를 상속받아 제작했으며 AI의 탐지범위 안에 Target(플레이어)가 있는지 검사하는 클래스입니다. UWorld 클래스에 있는 OverlapMultiByChannel을 사용해 Player 채널에 있는 폰을 검사하도록 설계했습니다. 해당 채널에 감지되는 폰이 있다면 Blackboard의 Target 변수에 해당 폰을 대입하고 감지 되지 않았다면 nullptr을 대입합니다.

<br>

## BTTask
UBTTask_Node를 상속받아 만든 클래스들입니다.

<br>

### BTTask_Attack
AI의 공격 행동을 지시하는 클래스입니다. AIInterface에 선언된 델리게이트와 함수를 먼저 보면

```cpp
DECLARE_DELEGATE(FAIMonsterAttackFinished)

virtual void AttackByAI(class UAnimMontage* InAnimMontage) = 0;
```

해당 코드들이 Attack 클래스와 함께 호출됩니다. 모든 AI들은 기본 클래스인 Enemy를 상속받게 됩니다. Enemy는 AIInterface를 구현하고 있습니다. 그래서 Enemy를 상속받은 AI들이 AttackByAI() 함수를 재정의하여 Montage 포인터를 넘겨주기만 하면 해당 몽타주를 실행하도록 설계했습니다. 

<br>

공격 몽타주가 끝나면 FAIMonsterAttackFinished 델리게이트를 실행하여 Attack 클래스가 Succeeded를 리턴하며 끝납니다.

<br>

### BTTask_TurnToTarget
AI가  공격을 진행할 때 Target(플레이어)를 바라볼 수 있게 하는 클래스입니다. RInterpTo() 보간 함수를 사용하여 Target을 향해 서서히 회전하도록 설계했습니다.

<br>

### BTTask_PatrolToFind
AI의 패트롤 기능을 담당하는 클래스입니다.

<br>

### BTTask_Aiming
궁수 몬스터의 활 시위를 당기는 연출을 하기 위한 클래스입니다. 시위를 당기는 몽타주가 끝나면 끝Succeeded를 리턴하며 끝납니다.

<br>

### BTTask_StopAnimation
궁수 몬스터의 Aiming 클래스가 진행되던 중 Target(플레이어)가 탐지 범위 밖으로 나갔을 시 활 시위를 당기던 몽타주를 끝내기 위한 클래스입니다. Aiming 클래스를 진행하며 손에 화살이 생성되게 되는데 그것 또한 삭제합니다.

</div>
</details>

<details>
<summary><h1>Notify</h1></summary>
<div markdown="1">

## 피격 판정을 체크하는 클래스
공격 몽타주 중간에 들어가 공격 판정을 하기 위한 클래스들입니다.
각 클래스들은 인터페이스를 통해 공격의 주체인 Character 클래스들과 연결되어 있습니다.

### AttackHitCheckNotify
캐릭터의 기본 공격에 대한 공격 판정입니다. 구현은 인터페이스를 통해 이루어집니다.

<br>

### JumpAttackDoneNotify
캐릭터의 점프 공격에 대한 공격 판정입니다. 구현은 인터페이스를 통해 이루어집니다.

<br>

### KickAttackNotify
캐릭터의 킥 공격에 대한 공격 판정입니다. 구현은 인터페이스를 통해 이루어집니다.

<br>

### BossAttackHitCheckAnimNotify
보스 몬스터의 기본 공격에 대한 공격 판정입니다. 구현은 인터페이스를 통해 이루어집니다.

<br>

### EnemyAttackHitCheckNotify
기본 몬스터의 기본 공격에 대한 공격 판정입니다. 구현은 인터페이스를 통해 이루어집니다.

<br>

## ArcherArrowSpawnNotify
해당 클래스는 궁수 몬스터의 활 시위를 당기고 발사하는 몽타주 중간에 화살을 생성하기 위한 클래스입니다. 인터페이스를 통해 궁수 몬스터와 연결되어 있습니다. 구현은 인터페이스를 통해 이루어집니다.

</div>
</details>

<details>
<summary><h1>Animation</h1></summary>
<div markdown="1">

## CharacterAnimInstance
캐릭터의 상태를 관찰하고 Animation_Blueprint에서 상태에 따라 애니메이션을 변경하기 위해 변수들을 저장하는 클래스입니다.
땅 위에서 움직임, 전투 상태, 방패를 든 상태, 사다리를 타고 있는지, 나이프를 조준하고 있는지 등을 NativeUpdateAnimation() 함수를 통해 업데이트 합니다. 

</div>
</details>



<details>
<summary><h1>Character</h1></summary>
<div markdown="1">

## ThroneCharacter
캐릭터 클래스는 입력 처리를 담당하고 있으며, 기능들은 최대한 컴포넌트에 구현해 커플링을 지양하고 클래스가 방대해 지는 것을 막고자 하였습니다.

<br>

### 컴포넌트

실질적인 기능 구현을 담당하고 있는 컴포넌트들입니다. 

- AbilityComponent
    - 전투부터 상호작용까지 캐릭터가 행하는 모든 행동을 구현하고 있습니다.
- StatComponent
    - 캐릭터의 스텟을 관리하는 컴포넌트입니다.

<br>

### 상태

캐릭터는 3가지의 상태를 가지며 각 상태에 따라 각기 다른 애니메이션과 입력을 가집니다.

- 기본(Default) 상태
    - 무기를 들고 있지 않은 상태며 게임이 시작할 때 갖는 기본 상태값 입니다.
- 전투(HoldWeapon) 상태
    - 무기를 들고 있는 상태며 해당 아이템 획득시 가질 수 있는 상태값 입니다.
    - 무기를 가진 상태라면 기본 상태와 전투 상태를 왔다갔다 할 수 있습니다.
- 사다리(Ladder) 상태
    - 사다리에 타있는 상태이며 움직임이 사다리를 타는 것으로 제한됩니다.

<br>

### 입력
입력은 기본 상태, 전투 상태, 사다리에 타있는 상태에 따라 변경됩니다.

기본, 전투 상태에서 공통 입력

- WASD - 움직임
- Mouse XY - 화면 전환
- SpaceBar - 점프
- E - 상호 작용
- F - 킥 공격
- Q - 무기 칼집에서 꺼내기 및 집어넣기

- 전투 상태
- Mouse Left Click - Knife 투척(조준 상태)
- Mouse Right Click - Knife 조준

무기 상태

- Mouse Left Click - 기본 공격, 점프 상태에선 내려 찍는 공격
- Mouse Right Click - 방어

사다리 상태

- WS - 위, 아래 움직임
- Mouse XY - 화면 전환
- E - 상호 작용

<br>

### 상호작용

무기 장착 상호작용을 위한 함수인 AcquisitionItem를 제외하고 나머지 상호작용들은 해당 상호작용 오브젝트의 BoxCollision 안에 들어갔을시 캐릭터가 가지고 있는 포인터에 대입되고 동작하도록 설계했습니다. 박스에서 벗어날 시 포인터는 nullptr 이 되며 상호작용 함수들에서 null 검사를 먼저 진행합니다. 또한 캐릭터가 상호작용 범위 안으로 들어갈시 UI가 표시됩니다.

- 레버 상호작용
- Door 상호작용
- DragonGate 상호작용
- 사다리 상호작용
- 무기 장착 상호작용
- 상호작용 범위 안으로 들어올시 나타나는 UI

<br>

### HUD

캐릭터의 현재 스텟에 따라 동적으로 변화하는 UI 입니다. Tick에서의 구현을 통해 일정 시간이 지나면 자동으로 회복되도록 구현했습니다.

- 체력바
- 기력바


</div>
</details>




<details>
<summary><h1>Component</h1></summary>
<div markdown="1">

...

</div>
</details>





<br>

<애니메이션 구조>
=============

