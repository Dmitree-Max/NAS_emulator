import random


def generate_boxes(amount, max_number=1000):
    result = []
    for i in range(amount):
        new_box = random.randint(1, max_number + 1)
        if new_box in result:
            i -= 1
        else:
            result.append(new_box)
    return result


def generate_disks(boxes, max_gatekeepers=2, max_other_disks=4, max_disk_number=63999):
    gatekeepers = []
    all_other_disk_am = 0
    box_disks = [0] * len(boxes)
    for j in range(len(boxes)):
        box_disks[j] = []
        box_syms = []
        gatekeepers_amount = random.randint(1, max_gatekeepers + 1)
        for i in range(gatekeepers_amount):
            gatekeepers_number = random.randint(1, max_disk_number)
            sym = random.randint(1, max_disk_number)
            if gatekeepers_number in gatekeepers or sym in box_syms:
                i -= 1
            else:
                gatekeepers.append(gatekeepers_number)
                box_disks[j].append((gatekeepers_number, sym))
        other_disk_amount = random.randint(0, max_other_disks + 1)
        for i in range(other_disk_amount):
            sym = random.randint(1, max_disk_number)
            if sym in box_syms:
                i -= 1
            else:
                all_other_disk_am += 1
                box_disks[j].append((0, sym))
    return gatekeepers, all_other_disk_am, box_disks


def main():
    with open("auto_config.txt", 'w') as log:
        boxes = generate_boxes(3)
        gatekeepers, all_other_disk_am, box_disks = generate_disks(boxes)
        log.write("Automatically generated configuration\n\n")
        log.write("Boxes: {")
        for i in range(len(boxes)):
            log.write("|")
            log.write(str(boxes[i]) + ": ")
            for disk in box_disks[i]:
                log.write("[disk " + str(disk[0]) + ", sym " + str(disk[1]) + "], ")
            log.write("|, ")
        log.write("}")


if __name__ == "__main__":
    main()
